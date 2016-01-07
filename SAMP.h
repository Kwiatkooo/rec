#ifndef _SAMP_H
#define _SAMP_H

namespace SAMP
{
	enum Version
	{
		VER_037R2,

		VER_TOTAL,
		VER_UNKNOWN = -1
	};

	enum Limit
	{
		L_MAX_OBJECTS,

		L_TOTAL
	};

	enum MemoryAddress
	{
		MA_D3DD9_PRESENT_HOOK_POS,
		MA_D3DD9_LOST_HOOK_POS,
		MA_D3DD9_RESET_HOOK_POS,
		MA_D3DD9,

		MA_CONNECT_ATTEMPT_DELAY,
		MA_USERFILES_SAMP_PATH,
		MA_SETENCRYPTIONPORT,
		
		MA_CNETGAME,
		MA_CNETGAME_GAMEMODERESTART,
		MA_CNETGAME_PACKET_DISCONNECTNOTIFICATION,
		MA_CNETGAME_PACKET_CONNECTIONLOST,
		MA_CNETGAME_GETPLAYERPOOL,
		MA_CNETGAME_GETOBJECTPOOL,
		MA_CNETGAME_GETRAKCLIENT,
		
		MA_PACKET_DISCONNECTNOTIFICATION_HOOK_POS,

		MA_CGAME,
		MA_CGAME_TOGGLESHOWCURSOR,
		MA_CGAME_ISMENUACTIVE,

		MA_CPLAYERPOOL_SETLOCALPLAYERNAME,
		MA_CPLAYERPOOL_GETLOCALPLAYER,

		MA_CLOCALPLAYER_SPAWN,

		MA_COBJECTPOOL_OBJECTEXISTS,
		MA_COBJECTPOOL_GETOBJECT,

		MA_CCMDWINDOW,
		MA_CCMDWINDOW_ADDCMDPROC,

		MA_CCHATWINDOW,
		MA_CCHATWINDOW_ADDCLIENTMESSAGE,

		MA_TOTAL
	};

	enum CNetAttribute
	{
		CNA_GAMESTATE,
		CNA_ADDRESS,
		CNA_PORT,
		CNA_RAKCLIENT,

		CNA_TOTAL
	};

	enum CObjectAttribute
	{
		COA_MODELID,
		COA_DRAWDISTANCE,
		COA_ROT,
		COA_POS,
		COA_ISMOVING,

		COA_TOTAL
	};

	struct VersionChecks
	{
		int iTextAddress;
		int iTextReference;
		char szText[64];
		Version version;
	};

	class CNetGame;
	class CGame;
	class CPlayerPool;
	class CLocalPlayer;
	class CObjectPool;
	class CObject;
	class CCmdWindow;
	class CChatWindow;
	class RakClientInterface;

	class Manager
	{
		friend class CNetGame;
		friend class CObject;
	private:
		static char *module;
		static Version version;
		static int init;
		static int Limits[ L_TOTAL ][ VER_TOTAL ];
		static unsigned int Offsets[ MA_TOTAL ][ VER_TOTAL ];
		static struct VersionChecks Checks[ VER_TOTAL ];
	private:
		static int Init();
	public:
		static Version DetermineVersion();
		static Version GetVersion();

		__inline static int GetLimit( Limit l )
		{
			return Limits[ l ][ version ];
		}

		template < typename T >
		__inline static T GetModule()
		{
			return (T)( module );
		}
		
		template < typename T >
		__inline static T GetAddressValue( MemoryAddress ma )
		{
			return (T)( version == VER_UNKNOWN ? NULL : module + Offsets[ ma ][ version ] );
		}

		template < typename T >
		__inline static T GetValueAt( MemoryAddress ma )
		{
			return (T)( version == VER_UNKNOWN ? NULL : *(T *)( module + Offsets[ ma ][ version ] ) );
		}

		template < typename T >
		__inline static T* GetPointer( MemoryAddress ma )
		{
			return (T *)( version == VER_UNKNOWN ? NULL : module + Offsets[ ma ][ version ] );
		}

		template < typename T >
		__inline static T* GetPointerAt( MemoryAddress ma )
		{
			return (T *)( version == VER_UNKNOWN ? NULL : *(T **)( module + Offsets[ ma ][ version ] ) );
		}
		
		template < typename T >
		__inline static T GetValueAtRAW( int raw )
		{
			return (T)( module == NULL ? NULL : *(T *)( module + raw ) );
		}

		template < typename T >
		__inline static T* GetPointerRAW( int raw )
		{
			return (T *)( module == NULL ? NULL : module + raw );
		}

		template < typename T >
		__inline static T* GetPointerAtRAW( int raw )
		{
			return (T *)( module == NULL ? NULL : *(T **)( module + raw ) );
		}
	};

	class CNetGame
	{
	private:
		static unsigned int CNetOffsets[ CNA_TOTAL ][ VER_TOTAL ];
	public:
		template < typename T >
		__inline T* GetAttributePointer( CNetAttribute cna )
		{
			return (T *)( this == NULL ? NULL : ( ( (char *)this ) + CNetOffsets[ cna ][ Manager::version ] ) );
		}

		template < typename T >
		__inline T GetAttribute( CNetAttribute cna )
		{
			T *ptr = GetAttributePointer< T >( cna );
			return ( T )( ptr == NULL ? NULL : *ptr );
		}

		template < typename T >
		__inline bool SetAttribute( CNetAttribute cna, T value )
		{
			T *ptr =  GetAttributePointer< T >( cna );
			if ( ptr != NULL )
				*ptr = value;

			return ptr != NULL;
		}

		__inline bool SetAttributeArray( CNetAttribute cna, void *pArray, unsigned int uiSize )
		{
			void *ptr = GetAttributePointer< void >( cna );
			if ( ptr != NULL )
			{
				memcpy( ptr, pArray, uiSize );
			}

			return ptr != NULL;
		}

		static void Init();

		void GameModeRestart();
		void Packet_DisconnectNotification();
		void Packet_ConnectionLost();
		CPlayerPool* GetPlayerPool();
		CObjectPool* GetObjectPool();
		RakClientInterface* GetRakClient();
		void SetServer( char *szAddress, int iPort );
	};

	class CGame
	{
	public:
		void ToggleShowCursor( bool bShow );
	};

	class CPlayerPool
	{
	public:
		void SetLocalPlayerName( char *szPlayerName );
		CLocalPlayer* GetLocalPlayer();
	};

	class CLocalPlayer
	{
	public:
		void Spawn();
	};

	class CObjectPool
	{
	public:
		bool ObjectExists( short id );
		CObject* GetObj( short id );
	};

	class CObject
	{
	private:
		static unsigned int CObjectOffsets[ COA_TOTAL ][ VER_TOTAL ];
	public:
		template < typename T >
		__inline T* GetAttributePointer( CObjectAttribute coa )
		{
			return (T *)( this == NULL ? NULL : ( ( (char *)this ) + CObjectOffsets[ coa ][ Manager::version ] ) );
		}

		template < typename T > 
		__inline T GetAttribute( CObjectAttribute coa )
		{
			T *ptr = GetAttributePointer< T >( coa );
			return ( T )( ptr == NULL ? NULL : *ptr );
		}

		static void Init();
		
		int GetModelID();
		float* GetPos();
		float GetX();
		float GetY();
		float GetZ();
		float* GetRot();
		float GetRotX();
		float GetRotY();
		float GetRotZ();
		float GetDrawDistance();
		bool IsMoving();
	};

	class CCmdWindow
	{
	public:
		void AddCmdProc( PCHAR szCmdName, PVOID cmdHandler );
	};

	class CChatWindow
	{
	public:
		void AddClientMessage( DWORD dwColor, PCHAR szStr );
	};

	class RakClientInterface
	{
	public:
		/// Destructor
		virtual ~RakClientInterface() {}

		///Call this to connect the client to the specified host (ip or domain name) and server port.
		/// This is a non-blocking connection.  You know the connection is successful when IsConnected() returns true
		/// or receive gets a packet with the type identifier ID_CONNECTION_REQUEST_ACCEPTED.
		/// \param[in] host Dotted IP or a domain name
		/// \param[in] serverPort The port on which to connect to \a host 
		/// \param[in] clientPort The port to use localy 
		/// \param[in] depreciated Ignore this
		/// \param[in] threadSleepTimer How many ms to Sleep each internal update cycle (30 to give the game priority, 0 for regular (recommended), -1 to not Sleep() (may be slower))
		/// \return true on successful initiation, false otherwise
		virtual bool Connect( const char* host, unsigned short serverPort, unsigned short clientPort, unsigned int depreciated, int threadSleepTimer ) = 0;

		/// Stops the client, stops synchronized data, and resets all internal data.
		/// Does nothing if the client is not connected to begin wit
		/// \param[in] blockDuration how long you should wait for all remaining packets to go outIf you set it to 0 then the disconnection notification probably won't arrive
		/// \param[in] orderingChannel If blockDuration > 0, the disconnect packet will be sent on this channel
		virtual void Disconnect( unsigned int blockDuration, unsigned char orderingChannel = 0 ) = 0;

		/// Can be called to use specific public RSA keys. (e and n)
		/// In order to prevent altered keys.
		/// Will return ID_RSA_PUBLIC_KEY_MISMATCH in a packet if a key has been altered.
		/// \param[in] privKeyP Private keys generated from the RSACrypt class. Can be 0
		/// \param[in] privKeyQ Private keys generated from the RSACrypt class. Can be 0 
		/// \sa Encryption.cpp	
		virtual void InitializeSecurity( const char *privKeyP, const char *privKeyQ ) = 0;

		/// Set the password to use when connecting to a server.  The password persists between connections.
		/// \param[in] _password The password to use to connect to a server, or 0 for none.
		virtual void SetPassword( const char *_password ) = 0;

		/// Returns true if a password was set, false otherwise
		/// \return true if a password has previously been set using SetPassword
		virtual bool HasPassword( void ) const = 0;
	};
};

#endif