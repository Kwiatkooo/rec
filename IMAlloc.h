#ifndef _IMALLOC_H
#define _IMALLOC_H

class IMAlloc
{
public:
	virtual ~IMAlloc() {}
	virtual bool Allocate( unsigned int uiSize ) = 0;
	virtual bool Free() = 0;
	virtual void* GetMemoryPointer() = 0;
};

#endif