#ifndef _CCHEAT_H
#define _CCHEAT_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <vector>

#include "CSingleton.h"
#include "GTASA.h"
#include "SAMP.h"
#include "IHook.h"
#include "IPatch.h"
#include "MemoryHackingFactory.h"
#include "CMapRecorder.h"

class CCheat : public CSingleton< CCheat >
{
	friend class CSingleton< CCheat >;

	enum HookId
	{
		HI_ONRENDER,
		HI_ONDEVICELOST,
		HI_ONDEVICERESET,

		HI_TOTAL
	};

	enum PatchId
	{
		PI_TOTAL
	};
private:
	IHook *m_pLoadLibraryHook;
	std::vector< IHook* > m_vHooks;
	std::vector< IPatch* > m_vPatches;
	bool m_bInitialized;
	WNDPROC m_pOldWindowProc;
	SAMP::CGame *m_pGame;
	SAMP::CNetGame *m_pNetGame;
	SAMP::CCmdWindow *m_pCmdWindow;
	SAMP::CChatWindow *m_pChatWindow;
public:
	SAMP::CGame* Game()
	{
		return m_pGame;
	}

	SAMP::CNetGame* NetGame()
	{
		return m_pNetGame;
	}

	SAMP::CCmdWindow* CmdWindow()
	{
		return m_pCmdWindow;
	}

	SAMP::CChatWindow* ChatWindow()
	{
		return m_pChatWindow;
	}
private:
	CCheat();
	~CCheat();
	void PreInit();
	void InstallHooks();
	void RegisterPatches();
	void InitCheat();
	void DestroyCheat();

	static void OnLoadLibrary();
	static void OnRender();
	static void OnDeviceLost();
	static void OnDeviceReset();
	static LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};

#endif