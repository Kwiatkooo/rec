#ifndef _CHOOK_H
#define _CHOOK_H

#include "IMProt.h"
#include "IMAlloc.h"
#include "IHook.h"

class CHook : public IHook
{
private:
	void *m_lpProcedure;
	void *m_lpCallback;
	unsigned int m_uiNumBytes;
	HookType m_Type;
	bool m_bInstalled;
	void *m_lpReturnAddress;
	void *m_lpTrampolineAddress;
	void *m_lpBack;
	IMProt *m_pMProt;
	IMAlloc *m_pMAlloc;
public:
	CHook();
	~CHook();
	bool Register( void *lpProcedure, void *lpCallback, unsigned int uiNumBytes, HookType type );
	bool Register( void *lpVTBL, void *lpCallback, unsigned int index );
	bool Install();
	bool Uninstall();
	bool Installed();
	void* GetReturnAddress();
	void* GetTrampolineAddress();
};

#endif