#include <Windows.h>
#include "CMProt.h"

CMProt::CMProt()
{
	m_lpAddress = NULL;
	m_uiSize = 0;
	m_uiPageSize = 0;

	SYSTEM_INFO info;
	GetSystemInfo( &info );

	m_uiPageSize = info.dwPageSize;
}

CMProt::~CMProt()
{
}

bool CMProt::Register( void *lpAddress, unsigned int uiSize )
{
	bool bOk = !( lpAddress == NULL || uiSize == 0 || m_lpAddress != NULL );
	
	if ( bOk )
	{
		m_lpAddress = lpAddress;
		m_uiSize = uiSize;
	}

	return bOk;
}

bool CMProt::Protect( MemoryProtection mp )
{
	unsigned int prot;
	bool bOk = m_lpAddress != NULL && m_uiSize > 0;

	if ( bOk )
	{
		switch ( mp )
		{
		case MP_READ:
			prot = PAGE_READONLY;
			break;
		case MP_WRITE:
			prot = PAGE_WRITECOPY;
			break;
		case MP_EXECUTE:
			prot = PAGE_EXECUTE;
			break;
		case MP_READ_WRITE:
			prot = PAGE_READWRITE;
			break;
		case MP_READ_EXECUTE:
			prot = PAGE_EXECUTE_READ;
			break;
		case MP_READ_WRITE_EXECUTE:
			prot = PAGE_EXECUTE_READWRITE;
			break;
		default:
			bOk = false;
		}

		if ( bOk )
		{
			DWORD dwOld;
			bOk = VirtualProtect( m_lpAddress, m_uiSize, prot, &dwOld ) != 0;
		}
	}

	return bOk;
}

unsigned int CMProt::GetPageSize()
{
	return m_uiPageSize;
}