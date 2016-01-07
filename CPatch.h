#ifndef _CPATCH_H
#define _CPATCH_H

#include "IMProt.h"
#include "IPatch.h"

class CPatch : public IPatch
{
private:
	void *m_lpAddress;
	void *m_lpPatch;
	unsigned int m_uiSize;
	bool m_bInstalled;
	void *m_lpBack;
	IMProt* m_pMProt;
public:
	CPatch();
	~CPatch();
	bool Register( void *lpAddress, void *lpPatch, unsigned int uiSize );
	bool Install();
	bool Uninstall();
	bool Installed();
};

#endif