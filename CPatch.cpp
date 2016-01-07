#include <memory>
#include "CPatch.h"
#include "MemoryHackingFactory.h"

CPatch::CPatch()
{
	m_lpAddress = NULL;
	m_lpPatch = NULL;
	m_uiSize = 0;
	m_bInstalled = false;
	m_lpBack = NULL;
	m_pMProt = MemoryHackingFactory::GetMProt();
}

CPatch::~CPatch()
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
}

bool CPatch::Register( void *lpAddress, void *lpPatch, unsigned int uiSize )
{
	bool bOk = 
		!
		( 
			lpAddress == NULL || lpPatch == NULL || m_pMProt == NULL ||
			uiSize == 0 || uiSize > m_pMProt->GetPageSize() || m_lpBack != NULL
		);

	if ( bOk )
	{
		bOk = m_pMProt->Register( lpAddress, uiSize );
	}

	if ( bOk )
	{
		m_lpAddress = lpAddress;
		m_lpPatch = lpPatch;
		m_lpBack = new char[ uiSize ];
		m_uiSize = uiSize;

		bOk = m_lpBack != NULL;
	}

	return bOk;
}

bool CPatch::Install()
{
	bool bOk = !Installed() && m_lpBack != NULL;

	if ( bOk )
	{
		bOk = m_pMProt->Protect( MP_READ_WRITE_EXECUTE );
	}

	if ( bOk )
	{
		memcpy( m_lpBack, m_lpAddress, m_uiSize );
		memcpy( m_lpAddress, m_lpPatch, m_uiSize ); 
		
		m_bInstalled = true;
	}

	return bOk;
}

bool CPatch::Uninstall()
{
	bool bOk = Installed() && m_lpBack != NULL;

	if ( bOk )
	{
		bOk = m_pMProt->Protect( MP_READ_WRITE_EXECUTE );
	}

	if ( bOk )
	{
		memcpy( m_lpAddress, m_lpBack, m_uiSize );
		m_bInstalled = false;
	}

	return bOk;
}

bool CPatch::Installed()
{
	return m_bInstalled;
}