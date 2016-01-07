#include "CCheat.h"

CCheat::CCheat()
{
	m_bInitialized = false;
	m_pLoadLibraryHook = NULL;

	if ( SAMP::Manager::GetModule< HMODULE >() == NULL )
	{
		m_pLoadLibraryHook = MemoryHackingFactory::GetHook();
		m_pLoadLibraryHook->Register( 
			GetProcAddress( LoadLibrary( TEXT( "Kernel32.dll" ) ), "LoadLibraryA" ),
			OnLoadLibrary,
			5,
			HT_TRAMPOLINE_AUTO
			);
		m_pLoadLibraryHook->Install();
	}
	else
	{
		PreInit();
	}
}

CCheat::~CCheat()
{
	if ( !m_vHooks.empty() )
	{
		for ( int i = 0; i < HI_TOTAL; i++ )
		{
			MemoryHackingFactory::DestroyHook( m_vHooks[ i ] );
		}

		m_vHooks.clear();
	}

	DestroyCheat();

	if ( !m_vPatches.empty() )
	{
		for ( int i = 0; i < PI_TOTAL; i++ )
		{
			MemoryHackingFactory::DestroyPatch( m_vPatches[ i ] );
		}

		m_vPatches.clear();
	}

	if ( m_pLoadLibraryHook != NULL )
	{
		MemoryHackingFactory::DestroyHook( m_pLoadLibraryHook );
		m_pLoadLibraryHook = NULL;
	}
}

void CCheat::PreInit()
{
	if ( SAMP::Manager::GetVersion() != SAMP::VER_UNKNOWN )
	{
		InstallHooks();
		RegisterPatches();
	}
	else
	{
		MessageBox(
			NULL, 
			TEXT("Unknown SA-MP version."),
			TEXT("Cheat"), 
			MB_OK | MB_ICONERROR
			);
	}
}

void CCheat::InstallHooks()
{
	for ( int i = 0; i < HI_TOTAL; i++ )
		m_vHooks.push_back( MemoryHackingFactory::GetHook() );

	m_vHooks[ HI_ONRENDER ]->Register(
		SAMP::Manager::GetPointer< void >( SAMP::MA_D3DD9_PRESENT_HOOK_POS ),
		OnRender,
		5,
		HT_TRAMPOLINE_AUTO
		);

	m_vHooks[ HI_ONDEVICELOST ]->Register(
		SAMP::Manager::GetPointer< void >( SAMP::MA_D3DD9_LOST_HOOK_POS ),
		OnDeviceLost,
		5,
		HT_TRAMPOLINE_AUTO
		);

	m_vHooks[ HI_ONDEVICERESET ]->Register(
		SAMP::Manager::GetPointer< void >( SAMP::MA_D3DD9_RESET_HOOK_POS ),
		OnDeviceReset,
		5,
		HT_TRAMPOLINE_AUTO
		);

	for ( int i = 0; i < HI_TOTAL; i++ )
		m_vHooks[ i ]->Install();
}

void CCheat::RegisterPatches()
{
	for ( int i = 0; i < PI_TOTAL; i++ )
		m_vPatches.push_back( MemoryHackingFactory::GetPatch() );
}

void CCheat::InitCheat()
{
	if ( !m_bInitialized )
	{
		m_pGame = SAMP::Manager::GetPointerAt< SAMP::CGame >( SAMP::MA_CGAME );
		m_pNetGame = SAMP::Manager::GetPointerAt< SAMP::CNetGame >( SAMP::MA_CNETGAME );
		m_pCmdWindow = SAMP::Manager::GetPointerAt< SAMP::CCmdWindow >( SAMP::MA_CCMDWINDOW );
		m_pChatWindow = SAMP::Manager::GetPointerAt< SAMP::CChatWindow >( SAMP::MA_CCHATWINDOW );

		HWND hWnd = GTASA::Manager::GetValueAt< HWND >( GTASA::MA_WINDOWHANDLE );
		m_pOldWindowProc = (WNDPROC)GetWindowLongPtr( hWnd, GWLP_WNDPROC );
		SetWindowLongPtr( hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc );

		CMapRecorder::Start();

		m_bInitialized = true;
	}
}

void CCheat::DestroyCheat()
{
	if ( m_bInitialized )
	{
		CMapRecorder::Shutdown();
		m_bInitialized = false;
	}
}

void CCheat::OnLoadLibrary()
{
	if ( SAMP::Manager::GetModule< HMODULE >() == NULL)
	{
		SAMP::Manager::DetermineVersion();
		if ( SAMP::Manager::GetModule< HMODULE >() != NULL )
		{
			Instance()->PreInit();
		}
	}
}

void CCheat::OnRender()
{
	if ( !Instance()->m_bInitialized && SAMP::Manager::GetPointerAt< SAMP::CNetGame >( SAMP::MA_CNETGAME ) != NULL )
		Instance()->InitCheat();

	if ( Instance()->m_bInitialized )
	{
		CMapRecorder::Instance()->Process();
	}
}

void CCheat::OnDeviceLost()
{
	if ( Instance()->m_bInitialized )
	{
	}
}

void CCheat::OnDeviceReset()
{
	if ( Instance()->m_bInitialized )
	{
	}
}

LRESULT CALLBACK CCheat::WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return CallWindowProc( Instance()->m_pOldWindowProc, hWnd, uMsg, wParam, lParam );
}