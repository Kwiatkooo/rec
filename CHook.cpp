#include <memory>
#include "CHook.h"
#include "IPatch.h"
#include "MemoryHackingFactory.h"

CHook::CHook()
{
	m_lpProcedure = NULL;
	m_lpCallback = NULL;
	m_uiNumBytes = 0;
	m_Type = HT_UNKNOWN;
	m_bInstalled = false;
	m_lpReturnAddress = NULL;
	m_lpTrampolineAddress = NULL;
	m_lpBack = NULL;
	m_pMProt = MemoryHackingFactory::GetMProt();
	m_pMAlloc = MemoryHackingFactory::GetMAlloc();
}

CHook::~CHook()
{
	if ( Installed() )
	{
		Uninstall();
	}

	if ( m_lpBack != NULL )
	{
		delete [] m_lpBack;
		m_lpBack = NULL;
	}

	if ( m_pMProt != NULL )
	{
		MemoryHackingFactory::DestroyMProt( m_pMProt );
		m_pMProt = NULL;
	}

	if ( m_pMAlloc != NULL )
	{
		MemoryHackingFactory::DestroyMAlloc( m_pMAlloc );
		m_pMProt = NULL;
	}
}

bool CHook::Register( void *lpProcedure, void *lpCallback, unsigned int uiNumBytes, HookType type )
{
	bool bOk = 
		!
		( 
			lpProcedure == NULL || lpCallback == NULL || m_pMProt == NULL || 
			m_pMAlloc == NULL || uiNumBytes < 5 || uiNumBytes > m_pMProt->GetPageSize() || m_lpBack != NULL
		);

	if ( bOk )
	{
		bOk = m_pMProt->Register( lpProcedure, uiNumBytes );
	}

	if ( bOk )
	{
		m_lpProcedure = lpProcedure;
		m_lpCallback = lpCallback;
		m_uiNumBytes = uiNumBytes;
		m_Type = type;
		m_lpBack = new char[ uiNumBytes ];

		bOk = m_lpBack != NULL;
	}

	return bOk;
}

bool CHook::Register( void *lpVTBL, void *lpCallback, unsigned int index )
{
	bool bOk = lpVTBL != NULL && lpCallback != NULL && m_lpBack == NULL;

	if ( bOk )
	{
		bOk = m_pMProt->Register( &( (void **)lpVTBL )[ index ], sizeof ( void* ) );
	}

	if ( bOk )
	{
		m_lpProcedure = lpVTBL;
		m_lpCallback = lpCallback;
		m_uiNumBytes = index;
		m_Type = HT_VTBL;
		m_lpBack = ( (void **)lpVTBL )[ index ];

		bOk = m_lpBack != NULL;
	}

	return bOk;
}

bool CHook::Install()
{
	unsigned char *FROM, *TO, *RETURN, *OFFSET_CALLBACK, *TRAMPOLINE, *OFFSET_FIX, *OFFSET_RETURN;
	bool bOk = !Installed() && m_lpBack != NULL;

	if ( bOk )
	{
		bOk = m_pMProt->Protect( MP_READ_WRITE_EXECUTE );
	}

	if ( bOk )
	{
		if ( m_Type != HT_VTBL )
		{
			memcpy( m_lpBack, m_lpProcedure, m_uiNumBytes );

			FROM = (unsigned char *)m_lpProcedure;
			TO = (unsigned char *)m_lpCallback;
			m_lpReturnAddress = FROM + m_uiNumBytes;
			RETURN = (unsigned char *)m_lpReturnAddress;
			OFFSET_CALLBACK = (unsigned char *)( TO - FROM - 5 );

			switch ( m_Type )
			{
			case HT_JMP:
				FROM[ 0 ] = 0xE9;
				( (unsigned char **)( FROM + 1 ) )[ 0 ] = OFFSET_CALLBACK;
				break;
			case HT_CALL:
				FROM[ 0 ] = 0xE8;
				( (unsigned char **)( FROM + 1 ) )[ 0 ] = OFFSET_CALLBACK;
				break;
			case HT_TRAMPOLINE:
				bOk = m_pMAlloc->Allocate( m_uiNumBytes + 1 + sizeof ( void* ) );
				if ( bOk )
				{
					m_lpTrampolineAddress = m_pMAlloc->GetMemoryPointer();
					TRAMPOLINE = (unsigned char*)m_lpTrampolineAddress;

					memcpy( TRAMPOLINE, m_lpBack, m_uiNumBytes );

					if ( FROM[ 0 ] == 0xE8 || FROM[ 0 ] == 0xE9 )
					{
						OFFSET_FIX = ( (unsigned char **)( FROM + 1 ) )[ 0 ];
						OFFSET_FIX = (unsigned char *)( OFFSET_FIX + ( (__int64)FROM ) - TRAMPOLINE );
						( (unsigned char **)( TRAMPOLINE + 1 ) )[ 0 ] = OFFSET_FIX;
					}					
					
					TRAMPOLINE += m_uiNumBytes;
					OFFSET_RETURN = (unsigned char *)( RETURN - TRAMPOLINE - 5 );
					TRAMPOLINE[ 0 ] = 0xE9;
					( (unsigned char **)( TRAMPOLINE + 1 ) )[ 0 ] = OFFSET_RETURN;

					FROM[ 0 ] = 0xE9;
					( (unsigned char **)( FROM + 1 ) )[ 0 ] = OFFSET_CALLBACK;
				}
				break;
			case HT_TRAMPOLINE_AUTO:
				bOk = m_pMAlloc->Allocate( 2 + 5 + 2 + m_uiNumBytes + 1 + sizeof ( void* ) );
				if ( bOk )
				{
					m_lpTrampolineAddress = m_pMAlloc->GetMemoryPointer();
					TRAMPOLINE = (unsigned char*)m_lpTrampolineAddress;

					*TRAMPOLINE++ = 0x60; //pushad
					*TRAMPOLINE++ = 0x9C; //pushfd
					*TRAMPOLINE = 0xE8;
					( (unsigned char **)( TRAMPOLINE + 1 ) )[ 0 ] = (unsigned char *)(TO - TRAMPOLINE - 5);
					TRAMPOLINE += 5;
					*TRAMPOLINE++ = 0x9D; //popfd
					*TRAMPOLINE++ = 0x61; //popad

					memcpy( TRAMPOLINE, m_lpBack, m_uiNumBytes );

					if ( FROM[ 0 ] == 0xE8 || FROM[ 0 ] == 0xE9 )
					{
						OFFSET_FIX = ( (unsigned char **)( FROM + 1 ) )[ 0 ];
						OFFSET_FIX = (unsigned char *)( OFFSET_FIX + ( (__int64)FROM ) - TRAMPOLINE );
						( (unsigned char **)( TRAMPOLINE + 1 ) )[ 0 ] = OFFSET_FIX;
					}					
					
					TRAMPOLINE += m_uiNumBytes;
					OFFSET_RETURN = (unsigned char *)( RETURN - TRAMPOLINE - 5 );
					TRAMPOLINE[ 0 ] = 0xE9;
					( (unsigned char **)( TRAMPOLINE + 1 ) )[ 0 ] = OFFSET_RETURN;

					TRAMPOLINE = (unsigned char*)m_lpTrampolineAddress;

					FROM[ 0 ] = 0xE9;
					( (unsigned char **)( FROM + 1 ) )[ 0 ] = (unsigned char *)(TRAMPOLINE - FROM - 5);
				}
				break;
			default:
				bOk = false;
			}
		}
		else
		{
			m_lpReturnAddress = m_lpBack = ( (void **)m_lpProcedure )[ m_uiNumBytes ];
			( (void **)m_lpProcedure )[ m_uiNumBytes ] = m_lpCallback;
		}
	}

	m_bInstalled = bOk;

	return bOk;
}

bool CHook::Uninstall()
{
	bool bOk = Installed() && m_lpBack != NULL;

	if ( bOk )
	{
		bOk = m_pMProt->Protect( MP_READ_WRITE_EXECUTE );
	}

	if ( bOk )
	{
		if ( m_Type != HT_VTBL )
		{
			memcpy( m_lpProcedure, m_lpBack, m_uiNumBytes );
			m_pMAlloc->Free();
		}
		else
		{
			( (void **)m_lpProcedure )[ m_uiNumBytes ] = m_lpBack;
		}

		m_lpReturnAddress = NULL;
		m_lpTrampolineAddress = NULL;
		m_bInstalled = false;
	}

	return bOk;
}

bool CHook::Installed()
{
	return m_bInstalled;
}

void* CHook::GetReturnAddress()
{
	return m_lpReturnAddress;
}

void* CHook::GetTrampolineAddress()
{
	return m_lpTrampolineAddress;
}