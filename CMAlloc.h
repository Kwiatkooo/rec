#ifndef _CMALLOC_H
#define _CMALLOC_H

#include "IMAlloc.h"

class CMAlloc : public IMAlloc
{
private:
	void *m_lpMemory;
	unsigned int m_uiSize;
public:
	CMAlloc();
	~CMAlloc();
	bool Allocate( unsigned int uiSize );
	bool Free();
	void* GetMemoryPointer();
};

#endif