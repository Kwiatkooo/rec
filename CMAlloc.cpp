#include <Windows.h>
#include "CMAlloc.h"

CMAlloc::CMAlloc()
{
	m_lpMemory = NULL;
	m_uiSize = NULL;
}

CMAlloc::~CMAlloc()
{
	if ( m_lpMemory != NULL )
	{
		Free();
	}
}

bool CMAlloc::Allocate( unsigned int uiSize )
{
	bool bOk = m_lpMemory == NULL && uiSize > 0;

	if ( bOk )
	{
		m_lpMemory = VirtualAlloc( NULL, uiSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

		bOk = m_lpMemory != NULL;
	}

	if ( bOk )
	{
		m_uiSize = uiSize;
	}

	return bOk;
}

bool CMAlloc::Free()
{
	bool bOk = m_lpMemory != NULL;

	if ( bOk )
	{
		bOk = VirtualFree( m_lpMemory, m_uiSize, MEM_DECOMMIT ) != 0;

		m_lpMemory = NULL;
		m_uiSize = 0;
	}

	return bOk;
}

void* CMAlloc::GetMemoryPointer()
{
	return m_lpMemory;
}