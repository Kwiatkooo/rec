#include <Windows.h>
#include "CCheat.h"

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpvReserved
)
{
	switch ( fdwReason )
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls( hinstDLL );
		CCheat::Start();
		break;
	case DLL_PROCESS_DETACH:
		CCheat::Shutdown();
		break;
	}

	return TRUE;
}