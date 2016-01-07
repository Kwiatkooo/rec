#ifndef _IHOOK_H
#define _IHOOK_H

enum HookType
{
	HT_JMP,
	HT_CALL,
	HT_TRAMPOLINE,
	HT_TRAMPOLINE_AUTO,
	HT_VTBL,

	HT_TOTAL,
	HT_UNKNOWN = -1
};

class IHook
{
public:
	virtual ~IHook() {}
	virtual bool Register( void *lpProcedure, void *lpCallback, unsigned int uiNumBytes, HookType type ) = 0;
	virtual bool Register( void *lpVTBL, void *lpCallback, unsigned int index ) = 0;
	virtual bool Install() = 0;
	virtual bool Uninstall() = 0;
	virtual bool Installed() = 0;
	virtual void* GetReturnAddress() = 0;
	virtual void* GetTrampolineAddress() = 0;
};

#endif