#include "CCheat.h"

CMapRecorder::CMapRecorder()
{
	m_bRecord = false;
	m_fRecDistance = 0;

	SAMP::CCmdWindow *pCmdWindow = CCheat::Instance()->CmdWindow();
	SAMP::CChatWindow *pChatWindow = CCheat::Instance()->ChatWindow();

	pCmdWindow->AddCmdProc( "mrstart", CMDStartRecording );
	pCmdWindow->AddCmdProc( "mrstop", CMDStopRecording );
	pCmdWindow->AddCmdProc( "mrsave", CMDSave );
	pCmdWindow->AddCmdProc( "mrreset", CMDReset );
	pCmdWindow->AddCmdProc( "mrhelp", CMDHelp );

	pChatWindow->AddClientMessage( -0, "=============================" );
	pChatWindow->AddClientMessage( -1, " MapRecorder by Noob Retard..." );
	pChatWindow->AddClientMessage( -1, " ...has been initialized." );
	pChatWindow->AddClientMessage( -1, " /mrhelp - show help");
	pChatWindow->AddClientMessage( -0, "=============================" );
}

CMapRecorder::~CMapRecorder()
{
	if ( m_mmObjects.size() > 0 )
		m_mmObjects.clear();
}

void CMapRecorder::Process()
{
	if ( m_bRecord )
	{
		SAMP::CObjectPool *pObjectPool = CCheat::Instance()->NetGame()->GetObjectPool();
		GTASA::CPed *pPlayerPed = GTASA::Manager::GetPointerAt< GTASA::CPed >( GTASA::MA_PLAYER_PED );
		SAMP::CChatWindow* pChatWindow = CCheat::Instance()->ChatWindow();

		if ( pObjectPool != NULL && pPlayerPed != NULL )
		{
			SAMP::CObject *pObject = NULL;
			int iMaxObjects = SAMP::Manager::GetLimit( SAMP::L_MAX_OBJECTS );
			stObject obj;

			for ( int i = 0; i <= iMaxObjects; i++ )
			{
				pObject = pObjectPool->GetObj( i );
				if ( pObject != NULL && !pObject->IsMoving() )
				{
					if ( m_fRecDistance == 0 ||  pPlayerPed->GetDistanceTo( pObject->GetPos() ) <= m_fRecDistance )
					{
						obj.mid = pObject->GetModelID();
						obj.x = pObject->GetX();
						obj.y = pObject->GetY();
						obj.z = pObject->GetZ();
						obj.rx = pObject->GetRotX();
						obj.ry = pObject->GetRotY();
						obj.rz = pObject->GetRotZ();
						obj.d = pObject->GetDrawDistance();


						bool bUnique = true;
						std::pair< std::multimap< short, stObject >::iterator, std::multimap< short, stObject >::iterator > range = m_mmObjects.equal_range( obj.mid );
						for ( std::multimap< short, stObject >::iterator it = range.first; bUnique && it != range.second; ++it )
						{
							if
							(
								IsAlmostEqual( it->second.x, obj.x ) &&
								IsAlmostEqual( it->second.y, obj.y ) &&
								IsAlmostEqual( it->second.z, obj.z ) &&
								IsAlmostEqual( it->second.rx, obj.rx ) &&
								IsAlmostEqual( it->second.ry, obj.ry ) &&
								IsAlmostEqual( it->second.rz, obj.rz ) &&
								IsAlmostEqual( it->second.d, obj.d )
							)
							{
								bUnique = false;
							}
									
						}

						if ( bUnique )
						{
							m_mmObjects.insert( std::make_pair( obj.mid, obj ) );
						}
					}
				}
			}
		}
	}
}

void CMapRecorder::CMDStartRecording( PCHAR szCmd )
{
	if ( !Instance()->m_bRecord )
	{
		SAMP::CChatWindow* pChatWindow = CCheat::Instance()->ChatWindow();
		char szMessage[ 128 ];

		if ( szCmd[0] != 0 )
		{
			Instance()->m_fRecDistance = (float)atof( szCmd );
			sprintf_s( szMessage, " MapRecorder: Started recording within distance: %f", Instance()->m_fRecDistance );
		}
		else
		{
			Instance()->m_fRecDistance = 0;
			strcpy_s( szMessage, "MapRecorder: Started recording within distance: INFINITE" );
		}

		pChatWindow->AddClientMessage( -0, "============================================================" );
		pChatWindow->AddClientMessage( -1, szMessage );
		pChatWindow->AddClientMessage( -0, "============================================================" );

		Instance()->m_bRecord = true;
	}	
}

void CMapRecorder::CMDStopRecording( PCHAR szCmd )
{
	if ( Instance()->m_bRecord )
	{
		Instance()->m_bRecord = false;

		SAMP::CChatWindow* pChatWindow = CCheat::Instance()->ChatWindow();

		char szMessage[ 128 ];
		sprintf_s( szMessage, " %u objects were recorded.", Instance()->m_mmObjects.size() );

		pChatWindow->AddClientMessage( -0, "================================" );
		pChatWindow->AddClientMessage( -1, " MapRecorder: Stopped recording." );
		pChatWindow->AddClientMessage( -1, szMessage );
		pChatWindow->AddClientMessage( -0, "================================" );
	}
}

void CMapRecorder::CMDSave( PCHAR szCmd )
{
	SAMP::CChatWindow* pChatWindow = CCheat::Instance()->ChatWindow();

	if ( Instance()->m_mmObjects.size() == 0 )
	{
		pChatWindow->AddClientMessage( -0, "===================================" );
		pChatWindow->AddClientMessage( -1, " Warning: No objects were recorded." );
		pChatWindow->AddClientMessage( -0, "===================================" );
		return;
	}

	CMDStopRecording( "" );

	char szFileName[ MAX_PATH ] = { 0 };
	char szFile[ MAX_PATH ] = { 0 };
	char format = 0;

	int nParams = sscanf_s( szCmd, "%s %c", szFileName, sizeof( szFileName ), &format, sizeof( format ) );

	if ( nParams < 1 )
	{
		pChatWindow->AddClientMessage( -0, "===================================" );
		pChatWindow->AddClientMessage( -1, " Usage: /mrsave <filename> [format]" );
		pChatWindow->AddClientMessage( -0, "===================================" );
		return;
	}

	const char *szSAMPUserFilesPath = SAMP::Manager::GetPointer< char >( SAMP::MA_USERFILES_SAMP_PATH );
	strcat_s( szFile, szSAMPUserFilesPath );
	strcat_s( szFile, "\\" );
	strcat_s( szFile, szFileName );
	strcat_s( szFile, ".txt" );

	FILE *pFile;
	fopen_s( &pFile, szFile, "w" );

	if ( pFile == NULL )
	{
		pChatWindow->AddClientMessage( -0, "===================================" );
		pChatWindow->AddClientMessage( -1, " Error: Could not create file." );
		pChatWindow->AddClientMessage( -0, "===================================" );
		return;
	}

	for ( std::multimap< short, stObject >::iterator it = Instance()->m_mmObjects.begin(); it != Instance()->m_mmObjects.end(); ++it )
	{
		switch ( format )
		{
		case 'd':
			fprintf_s( pFile, "CreateDynamicObject(%hd, %f, %f, %f, %f, %f, %f);\n", it->first,
				it->second.x, it->second.y, it->second.z,
				it->second.rx, it->second.ry, it->second.rz );
			break;
		default:
			fprintf_s( pFile, "CreateObject(%hd, %f, %f, %f, %f, %f, %f);\n", it->first,
				it->second.x, it->second.y, it->second.z,
				it->second.rx, it->second.ry, it->second.rz );
		};
	}

	fclose( pFile );

	Instance()->m_mmObjects.clear();

	pChatWindow->AddClientMessage( -0, "===================================" );
	pChatWindow->AddClientMessage( -1, " MapRecorder: objects saved!" );
	pChatWindow->AddClientMessage( -0, "===================================" );
}

void CMapRecorder::CMDReset( PCHAR szCmd )
{
	SAMP::CChatWindow* pChatWindow = CCheat::Instance()->ChatWindow();

	if ( Instance()->m_mmObjects.size() > 0 )
	{
		Instance()->m_mmObjects.clear();

		pChatWindow->AddClientMessage( -0, "===================================" );
		pChatWindow->AddClientMessage( -1, " MapRecorder: Memory cleared." );
		pChatWindow->AddClientMessage( -0, "===================================" );
	}
}

void CMapRecorder::CMDHelp( PCHAR szCmd )
{
	SAMP::CChatWindow* pChatWindow = CCheat::Instance()->ChatWindow();

	pChatWindow->AddClientMessage( -0, "===============================" );
	pChatWindow->AddClientMessage( -1, " MapRecorder by Noob Retard." );
	pChatWindow->AddClientMessage( -0, "==================================================" );
	pChatWindow->AddClientMessage( -1, " /mrstart [distance]" );
	pChatWindow->AddClientMessage( -1, " Starts objects recording within specified distance." );
	pChatWindow->AddClientMessage( -1, " If distance is not set or 0 then ignored." );
	pChatWindow->AddClientMessage( -0, "==================================================" );
	pChatWindow->AddClientMessage( -1, " /mrstop" );
	pChatWindow->AddClientMessage( -1, " Stops objects recording." );
	pChatWindow->AddClientMessage( -0, "==========================================================================" );
	pChatWindow->AddClientMessage( -1, " /mrsave <filename> [format]" );
	pChatWindow->AddClientMessage( -1, " Saves recorded objects to the specified file and resets MapRecorder state." );
	pChatWindow->AddClientMessage( -1, " Filename must be set. Example: \"/mrsave objects\" saves as objects.txt" );
	pChatWindow->AddClientMessage( -1, " The location where files are stored is GTA San Andreas User Files\\SAMP" );
	pChatWindow->AddClientMessage( -1, " The [format] param is optional and states for output format." );
	pChatWindow->AddClientMessage( -1, " Available formats:" );
	pChatWindow->AddClientMessage( -1, " d - CreateDynamicObject format" );
	pChatWindow->AddClientMessage( -1, " If [format] is not set or different than above then CreateObject is used" );
	pChatWindow->AddClientMessage( -0, "==========================================================================" );
	pChatWindow->AddClientMessage( -1, " /mrreset" );
	pChatWindow->AddClientMessage( -1, " Resets MapRecorder state, so recorded objects are removed from memory." );
	pChatWindow->AddClientMessage( -0, "==========================================================================" );

}