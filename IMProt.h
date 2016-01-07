#ifndef _IMPROT_H
#define _IMPROT_H

enum MemoryProtection
{
	MP_READ,
	MP_WRITE,
	MP_EXECUTE,
	MP_READ_WRITE,
	MP_READ_EXECUTE,
	MP_READ_WRITE_EXECUTE,

	MP_TOTAL,
	MP_UNKNOWN = -1
};

class IMProt
{
public:
	virtual ~IMProt() {}
	virtual bool Register( void *lpAddress, unsigned int uiSize ) = 0;
	virtual bool Protect( MemoryProtection mp ) = 0;
	virtual unsigned int GetPageSize() = 0;
};

#endif