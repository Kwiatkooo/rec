#include <Windows.h>
#include "SAMP.h"

namespace SAMP
{
	Version Manager::version = Manager::DetermineVersion();
	char *Manager::module = NULL;
	int Manager::init = Manager::Init();

	int Manager::Limits[ L_TOTAL ][ VER_TOTAL ];
	unsigned int Manager::Offsets[ MA_TOTAL ][ VER_TOTAL ];
	
	struct VersionChecks Manager::Checks[ VER_TOTAL ] =
	{
		{ 0xD4360, 0x8260F, "SA-MP 0.3.7", VER_037R2 }
	};

	int Manager::Init()
	{
		Limits[ L_MAX_OBJECTS ][ VER_037R2 ] = 1000;

		Offsets[ MA_D3DD9_PRESENT_HOOK_POS ][ VER_037R2 ] = 0x870AB;
		Offsets[ MA_D3DD9_LOST_HOOK_POS ][ VER_037R2 ] = 0x85A31;
		Offsets[ MA_D3DD9_RESET_HOOK_POS ][ VER_037R2 ] = 0x85A3C;
		Offsets[ MA_D3DD9 ][ VER_037R2 ] = 0x212A30;
		Offsets[ MA_CONNECT_ATTEMPT_DELAY ][ VER_037R2 ] = 0x2AE035;
		Offsets[ MA_USERFILES_SAMP_PATH ][ VER_037R2 ] = 0x212910;
		Offsets[ MA_SETENCRYPTIONPORT ][ VER_037R2 ] = 0x19870;
		Offsets[ MA_CNETGAME ][ VER_037R2 ] = 0x212A80;
		Offsets[ MA_CNETGAME_GAMEMODERESTART ][ VER_037R2 ] = 0x9280;
		Offsets[ MA_CNETGAME_PACKET_DISCONNECTNOTIFICATION ][ VER_037R2 ] = 0x7BE0;
		Offsets[ MA_CNETGAME_PACKET_CONNECTIONLOST ][ VER_037R2 ] = 0x9A60;
		Offsets[ MA_CNETGAME_GETPLAYERPOOL ][ VER_037R2 ] = 0x1170;
		Offsets[ MA_CNETGAME_GETOBJECTPOOL ][ VER_037R2 ] = 0x29E0;
		Offsets[ MA_CNETGAME_GETRAKCLIENT ][ VER_037R2 ] = 0x1620;
		Offsets[ MA_PACKET_DISCONNECTNOTIFICATION_HOOK_POS ][ VER_037R2 ] = 0xA0C4;
		Offsets[ MA_CGAME ][ VER_037R2 ] = 0x212A94;
		Offsets[ MA_CNETGAME_PACKET_CONNECTIONLOST ][ VER_037R2 ] = 0x63E20;
		Offsets[ MA_CGAME_TOGGLESHOWCURSOR ][ VER_037R2 ] = 0x63E20;
		Offsets[ MA_CGAME_ISMENUACTIVE ][ VER_037R2 ] = 0x64050;
		Offsets[ MA_CPLAYERPOOL_SETLOCALPLAYERNAME ][ VER_037R2 ] = 0xA650;
		Offsets[ MA_CPLAYERPOOL_GETLOCALPLAYER ][ VER_037R2 ] = 0x1610;
		Offsets[ MA_CLOCALPLAYER_SPAWN ][ VER_037R2 ] = 0x36D0;
		Offsets[ MA_COBJECTPOOL_OBJECTEXISTS ][ VER_037R2 ] = 0xE1F0;
		Offsets[ MA_COBJECTPOOL_GETOBJECT ][ VER_037R2 ] = 0x29B0;
		Offsets[ MA_CCMDWINDOW ][ VER_037R2 ] = 0x212A70;
		Offsets[ MA_CCMDWINDOW_ADDCMDPROC ][ VER_037R2 ] = 0x7BC40;
		Offsets[ MA_CCHATWINDOW ][ VER_037R2 ] = 0x212A6C;
		Offsets[ MA_CCHATWINDOW_ADDCLIENTMESSAGE ][ VER_037R2 ] = 0x7AA80;

		CNetGame::Init();

		return 1;
	}

	Version Manager::DetermineVersion()
	{
		module = (char *)GetModuleHandle( TEXT( "samp.dll" ) );

		version = VER_UNKNOWN;

		if ( module != NULL )
		{
			for ( int i = 0; version == VER_UNKNOWN && i != VER_TOTAL; i++ )
			{
				if 
				( 
					IsBadReadPtr( Manager::GetPointerRAW< void >( Checks[ i ].iTextAddress ), sizeof ( void* ) ) == FALSE &&
					IsBadReadPtr( Manager::GetPointerRAW< void >( Checks[ i ].iTextReference ), sizeof ( void* ) ) == FALSE
				)
				{
					if ( Manager::GetPointerRAW< void >( Checks[ i ].iTextAddress ) ==  Manager::GetPointerAtRAW< void >( Checks[ i ].iTextReference ) )
					{
						if ( strcmp( Manager::GetPointerAtRAW< char >( Checks[ i ].iTextReference ), Checks[ i ].szText ) == 0 )
						{
							version = Checks[ i ].version;
						}
					}
				}
			}
		}

		return version;
	}

	Version Manager::GetVersion()
	{
		return version;
	}

	//
	unsigned int CNetGame::CNetOffsets[ CNA_TOTAL ][ VER_TOTAL ];

	void CNetGame::Init()
	{
		CNetGame::CNetOffsets[ CNA_GAMESTATE ][ VER_037R2 ] = 0x190;
		CNetGame::CNetOffsets[ CNA_ADDRESS ][ VER_037R2 ] = 0x1C4;
		CNetGame::CNetOffsets[ CNA_PORT ][ VER_037R2 ] = 0x3C9;
		CNetGame::CNetOffsets[ CNA_RAKCLIENT ][ VER_037R2 ] = 0x3D1;

		CObject::Init();
	}

	typedef void ( __thiscall *GameModeRestart_t )( CNetGame* );
	typedef void ( __thiscall *Packet_t )( CNetGame*, void* );
	typedef void* ( __thiscall *GetC_t )( void* );
	
	void CNetGame::GameModeRestart()
	{
		return Manager::GetAddressValue< GameModeRestart_t >( MA_CNETGAME_GAMEMODERESTART )( this );
	}

	void CNetGame::Packet_DisconnectNotification()
	{
		return Manager::GetAddressValue< Packet_t >( MA_CNETGAME_PACKET_DISCONNECTNOTIFICATION )( this, NULL );
	}

	void CNetGame::Packet_ConnectionLost()
	{
		return Manager::GetAddressValue< Packet_t >( MA_CNETGAME_PACKET_CONNECTIONLOST )( this, NULL );
	}

	CPlayerPool* CNetGame::GetPlayerPool()
	{
		return (CPlayerPool *)Manager::GetAddressValue< GetC_t >( MA_CNETGAME_GETPLAYERPOOL )( this );
	}

	CObjectPool* CNetGame::GetObjectPool()
	{
		return (CObjectPool *)Manager::GetAddressValue< GetC_t >( MA_CNETGAME_GETOBJECTPOOL )( this );
	}

	RakClientInterface* CNetGame::GetRakClient()
	{
		return (RakClientInterface *)Manager::GetAddressValue< GetC_t >( MA_CNETGAME_GETRAKCLIENT )( this );
	}

	void CNetGame::SetServer( char *szAddress, int iPort )
	{
		SetAttribute< int >( CNA_PORT, iPort );
		SetAttributeArray( CNA_ADDRESS, szAddress, strlen( szAddress ) + 1 );
	}

	//
	typedef void ( __thiscall *ToggleShowCursor_t )( CGame*, int iParam1, int iParam2, int iParam3 );

	void CGame::ToggleShowCursor( bool bShow )
	{
		if ( bShow )
		{
			Manager::GetAddressValue< ToggleShowCursor_t >( MA_CGAME_TOGGLESHOWCURSOR )( this, 2, 0, 0 );
		}
		else
		{
			Manager::GetAddressValue< ToggleShowCursor_t >( MA_CGAME_TOGGLESHOWCURSOR )( this, 0, 1, 0 );
		}
	}

	//
	typedef void ( __thiscall *SetLocalPlayerName_t )( CPlayerPool*, char* );

	void CPlayerPool::SetLocalPlayerName( char *szPlayerName )
	{
		return Manager::GetAddressValue< SetLocalPlayerName_t >( MA_CPLAYERPOOL_SETLOCALPLAYERNAME )( this, szPlayerName );
	}

	CLocalPlayer* CPlayerPool::GetLocalPlayer()
	{
		return (CLocalPlayer *)Manager::GetAddressValue< GetC_t >( MA_CPLAYERPOOL_GETLOCALPLAYER )( this );
	}

	typedef void ( __thiscall *Spawn_t )( CLocalPlayer* );

	void CLocalPlayer::Spawn()
	{
		return Manager::GetAddressValue< Spawn_t >( MA_CLOCALPLAYER_SPAWN )( this );
	}

	//
	typedef bool ( __thiscall *ObjectExists_t )( CObjectPool*, short id );
	typedef CObject* ( __thiscall *GetObject_t )( CObjectPool*, short id );

	bool CObjectPool::ObjectExists( short id )
	{
		return Manager::GetAddressValue< ObjectExists_t >( MA_COBJECTPOOL_OBJECTEXISTS )( this, id );
	}

	CObject* CObjectPool::GetObj( short id )
	{
		return Manager::GetAddressValue< GetObject_t >( MA_COBJECTPOOL_GETOBJECT )( this, id );
	}

	//
	unsigned int CObject::CObjectOffsets[ COA_TOTAL ][ VER_TOTAL ];

	void CObject::Init()
	{
		CObjectOffsets[ COA_MODELID ][ VER_037R2 ] = 0x4E;
		CObjectOffsets[ COA_DRAWDISTANCE ][ VER_037R2 ] = 0x53;
		CObjectOffsets[ COA_ROT ][ VER_037R2 ] = 0xAC;
		CObjectOffsets[ COA_POS ][ VER_037R2 ] = 0x14A;
		CObjectOffsets[ COA_ISMOVING ][ VER_037R2 ] = 0x1AE;
	}

	int CObject::GetModelID()
	{
		return GetAttribute< int >( COA_MODELID );
	}

	float* CObject::GetPos()
	{
		return GetAttributePointer< float >( COA_POS );
	}

	float CObject::GetX()
	{
		return GetPos()[0];
	}

	float CObject::GetY()
	{
		return GetPos()[1];
	}

	float CObject::GetZ()
	{
		return GetPos()[2];
	}

	float* CObject::GetRot()
	{
		return GetAttributePointer< float >( COA_ROT );
	}

	float CObject::GetRotX()
	{
		return GetRot()[0];
	}

	float CObject::GetRotY()
	{
		return GetRot()[1];
	}

	float CObject::GetRotZ()
	{
		return GetRot()[2];
	}

	float CObject::GetDrawDistance()
	{
		return GetAttribute< float >( COA_DRAWDISTANCE );
	}

	bool CObject::IsMoving()
	{
		return GetAttribute< char >( COA_ISMOVING ) != 0;
	}

	//
	typedef void ( __thiscall *AddCmdProc_t )( CCmdWindow*, PCHAR szCmdName, PVOID cmdHandler );

	void CCmdWindow::AddCmdProc( PCHAR szCmdName, PVOID cmdHandler )
	{
		return Manager::GetAddressValue< AddCmdProc_t >( MA_CCMDWINDOW_ADDCMDPROC )( this, szCmdName, cmdHandler );
	}

	//
	typedef void ( __thiscall *AddClientMessage_t )( CChatWindow*, DWORD dwColor, PCHAR szStr );

	void CChatWindow::AddClientMessage( DWORD dwColor, PCHAR szStr )
	{
		return Manager::GetAddressValue< AddClientMessage_t >( MA_CCHATWINDOW_ADDCLIENTMESSAGE )( this, dwColor, szStr );
	}
};