#ifndef _CMPROT_H
#define _CMPROT_H

#include "IMProt.h"

class CMProt : public IMProt
{
private:
	void *m_lpAddress;
	unsigned int m_uiSize;
	unsigned int m_uiPageSize;
public:
	CMProt();
	~CMProt();
	bool Register( void *lpAddress, unsigned int uiSize );
	bool Protect( MemoryProtection mp );
	unsigned int GetPageSize();
};

#endif