#pragma once
#include "../Networking/Package.h"
#include "../Networking/Serializer.h"
#include "../Networking/Networking.h"
#include "../Networking/MessageTypes.h"

/////////////////////////////////////////////////////////////////////////////
//
// ChatServer
//
/////////////////////////////////////////////////////////////////////////////

class ChatServer
{
public:
	void init( uint16 nPort )
	{
		// Select instance to use.  For now we'll always use the default.
		// But we could use SteamGameServerNetworkingSockets() on Steam.
		m_pInterface = SteamNetworkingSockets();

		// Start listening
		SteamNetworkingIPAddr serverLocalAddr;
		serverLocalAddr.Clear();
		serverLocalAddr.m_port = nPort;
		SteamNetworkingConfigValue_t opt;
		opt.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback );
		m_hListenSock = m_pInterface->CreateListenSocketIP( serverLocalAddr, 1, &opt );
		if ( m_hListenSock == k_HSteamListenSocket_Invalid )
			FatalError( "Failed to listen on port %d", nPort );
		m_hPollGroup = m_pInterface->CreatePollGroup();
		if ( m_hPollGroup == k_HSteamNetPollGroup_Invalid )
			FatalError( "Failed to listen on port %d", nPort );
		Printf( "Server listening on port %d\n", nPort );

		// while ( !g_bQuit )
		// {

		// }

		// Close all the connections
	}
	void shutdown(){
		Printf( "Closing connections...\n" );
		for ( auto it: m_mapClients )
		{
			// Send them one more goodbye message.  Note that we also have the
			// connection close reason as a place to send final data.  However,
			// that's usually best left for more diagnostic/debug text not actual
			// protocol strings.
			SendStringToClient( it.first, "Server is shutting down.  Goodbye." );

			// Close the connection.  We use "linger mode" to ask SteamNetworkingSockets
			// to flush this out and close gracefully.
			m_pInterface->CloseConnection( it.first, 0, "Server Shutdown", true );
		}
		m_mapClients.clear();

		m_pInterface->CloseListenSocket( m_hListenSock );
		m_hListenSock = k_HSteamListenSocket_Invalid;

		m_pInterface->DestroyPollGroup( m_hPollGroup );
		m_hPollGroup = k_HSteamNetPollGroup_Invalid;
		ShutdownSteamDatagramConnectionSockets();
		NukeProcess(0);
	}
	void iteration(){
		PollIncomingMessages();
		PollConnectionStateChanges();
		PollLocalUserInput();
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}

	void sendToClient(HSteamNetConnection conn, std::vector<uint8_t> buffer){
		m_pInterface->SendMessageToConnection(conn, buffer.data(), buffer.size(), k_nSteamNetworkingSend_Reliable, nullptr);
	}

	void sendPackageToAllClients(Package package, HSteamNetConnection except = k_HSteamNetConnection_Invalid) {
		std::vector<uint8_t> buffer = package.serialize();
		for (const auto& client : m_mapClients) {
			HSteamNetConnection conn = client.first;
			if (conn != except) {
				sendToClient(conn, buffer);
			}
		}
	}
	void sendBufferToAllClients(std::vector<uint8_t> buffer, HSteamNetConnection except = k_HSteamNetConnection_Invalid ){
		for (const auto& client : m_mapClients) {
			HSteamNetConnection conn = client.first;
			if (conn != except) {
				sendToClient(conn, buffer);
			}
		}
	}

	HSteamNetConnection getConnectionByID(int playerID){
		for ( auto &c: m_mapClients )
		{
			if ( c.second.playerID == playerID )
				return c.first;
		}
		Printf("playerID %d not found", playerID);
		return HSteamNetConnection();
	}
	
private:
	int playerID_iterator = 1;
	HSteamListenSocket m_hListenSock;
	HSteamNetPollGroup m_hPollGroup;
	ISteamNetworkingSockets *m_pInterface;

	struct Client_t
	{
		int playerID;
		std::string m_sNick;
	};

	std::map< HSteamNetConnection, Client_t > m_mapClients;

	void SendStringToClient( HSteamNetConnection conn, const char *str )
	{
		//m_pInterface->SendMessageToConnection( conn, str, (uint32)strlen(str), k_nSteamNetworkingSend_Reliable, nullptr );
	}



	void SendStringToAllClients( const char *str, HSteamNetConnection except = k_HSteamNetConnection_Invalid )
	{
		// for ( auto &c: m_mapClients )
		// {
		// 	if ( c.first != except )
		// 		SendStringToClient( c.first, str );
		// }
	}

	// void beginGame(){
	// 	sendToAllClients(&currentGameState, 1);
	// }



	void PollIncomingMessages()
	{
		char temp[ 1024 ];
		while ( !g_bQuit )
		{
			ISteamNetworkingMessage *pIncomingMsg = nullptr;
			int numMsgs = m_pInterface->ReceiveMessagesOnPollGroup( m_hPollGroup, &pIncomingMsg, 1 ); // IMPORTANT this is where the message is recieved
			if ( numMsgs == 0 )
				break;
			if ( numMsgs < 0 )
				FatalError( "Error checking for messages" );
			assert( numMsgs == 1 && pIncomingMsg );
			auto itClient = m_mapClients.find( pIncomingMsg->m_conn );
			assert( itClient != m_mapClients.end() );
			
			for (int i = 0; i < 8 && i < pIncomingMsg->m_cbSize; ++i)
				std::cerr << *((int*)pIncomingMsg->m_pData) << " ";

			// std::cerr << *((int*)pIncomingMsg->m_pData) << std::endl;
			Package* package = steamMessageToPackage(pIncomingMsg);
			
			
			sendPackageToAllClients(*package); // just forward the message
			std::cerr << "message recieved and sent" << std::endl;
			pIncomingMsg->Release();


			// std::string sCmd;
			// sCmd.assign( (const char *)pIncomingMsg->m_pData, pIncomingMsg->m_cbSize ); //IMPORTANT this is where the message is translated to string i think
			// const char *cmd = sCmd.c_str();

			// We don't need this anymore.

			// Check for known commands.  None of this example code is secure or robust.
			// Don't write a real server like this, please.

			// if ( strncmp( cmd, "/nick", 5 ) == 0 )
			// {
			// 	const char *nick = cmd+5;
			// 	while ( isspace(*nick) )
			// 		++nick;

			// 	// Let everybody else know they changed their name
			// 	sprintf( temp, "%s shall henceforth be known as %s", itClient->second.m_sNick.c_str(), nick );
			// 	SendStringToAllClients( temp, itClient->first );

			// 	// Respond to client
			// 	sprintf( temp, "Ye shall henceforth be known as %s", nick );
			// 	SendStringToClient( itClient->first, temp );

			// 	// Actually change their name
			// 	SetClientNick( itClient->first, nick );
			// 	continue;
			// }

			// // Assume it's just a ordinary chat message, dispatch to everybody else
			// sprintf( temp, "%s: %s", itClient->second.m_sNick.c_str(), cmd );
			// SendStringToAllClients( temp, itClient->first );
		}


	}

	void PollLocalUserInput()
	{
		std::string cmd;
		while ( !g_bQuit && LocalUserInput_GetNext( cmd ))
		{
			if ( strcmp( cmd.c_str(), "/quit" ) == 0 )
			{
				g_bQuit = true;
				Printf( "Shutting down server" );
				break;
			}

			// That's the only command we support
			Printf( "The server only knows one command: '/quit'" );
		}
	}

	void SetClientNick( HSteamNetConnection hConn, const char *nick )
	{

		// Remember their nick
		m_mapClients[hConn].m_sNick = nick;

		// Set the connection name, too, which is useful for debugging
		m_pInterface->SetConnectionName( hConn, nick );
	}

	void SetClientPlayerID( HSteamNetConnection hConn)
	{
		m_mapClients[hConn].playerID = playerID_iterator++;
	}

	void OnSteamNetConnectionStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo )
	{
		char temp[1024];

		// What's the state of the connection?
		switch ( pInfo->m_info.m_eState )
		{
			case k_ESteamNetworkingConnectionState_None:
				// NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
				break;

			case k_ESteamNetworkingConnectionState_ClosedByPeer:
			case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
			{
				// Ignore if they were not previously connected.  (If they disconnected
				// before we accepted the connection.)
				if ( pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected )
				{

					// Locate the client.  Note that it should have been found, because this
					// is the only codepath where we remove clients (except on shutdown),
					// and connection change callbacks are dispatched in queue order.
					auto itClient = m_mapClients.find( pInfo->m_hConn );
					assert( itClient != m_mapClients.end() );

					// Select appropriate log messages
					const char *pszDebugLogAction;
					if ( pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally )
					{
						pszDebugLogAction = "problem detected locally";
						sprintf( temp, "Alas, %s hath fallen into shadow.  (%s)", itClient->second.m_sNick.c_str(), pInfo->m_info.m_szEndDebug );
					}
					else
					{
						// Note that here we could check the reason code to see if
						// it was a "usual" connection or an "unusual" one.
						pszDebugLogAction = "closed by peer";
						sprintf( temp, "%s hath departed", itClient->second.m_sNick.c_str() );
					}

					// Spew something to our own log.  Note that because we put their nick
					// as the connection description, it will show up, along with their
					// transport-specific data (e.g. their IP address)
					Printf( "Connection %s %s, reason %d: %s\n",
						pInfo->m_info.m_szConnectionDescription,
						pszDebugLogAction,
						pInfo->m_info.m_eEndReason,
						pInfo->m_info.m_szEndDebug
					);

					m_mapClients.erase( itClient );
					playerID_iterator -= 1;

					// Send a message so everybody else knows what happened
					SendStringToAllClients( temp );
				}
				else
				{
					assert( pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting );
				}

				// Clean up the connection.  This is important!
				// The connection is "closed" in the network sense, but
				// it has not been destroyed.  We must close it on our end, too
				// to finish up.  The reason information do not matter in this case,
				// and we cannot linger because it's already closed on the other end,
				// so we just pass 0's.
				m_pInterface->CloseConnection( pInfo->m_hConn, 0, nullptr, false );
				break;
			}

			case k_ESteamNetworkingConnectionState_Connecting:
			{
				// This must be a new connection
				assert( m_mapClients.find( pInfo->m_hConn ) == m_mapClients.end() );

				Printf( "Connection request from %s", pInfo->m_info.m_szConnectionDescription );

				// A client is attempting to connect
				// Try to accept the connection.
				if ( m_pInterface->AcceptConnection( pInfo->m_hConn ) != k_EResultOK )
				{
					// This could fail.  If the remote host tried to connect, but then
					// disconnected, the connection may already be half closed.  Just
					// destroy whatever we have on our side.
					m_pInterface->CloseConnection( pInfo->m_hConn, 0, nullptr, false );
					Printf( "Can't accept connection.  (It was already closed?)" );
					break;
				}

				// Assign the poll group
				if ( !m_pInterface->SetConnectionPollGroup( pInfo->m_hConn, m_hPollGroup ) )
				{
					m_pInterface->CloseConnection( pInfo->m_hConn, 0, nullptr, false );
					Printf( "Failed to set poll group?" );
					break;
				}

				// Generate a random nick.  A random temporary nick
				// is really dumb and not how you would write a real chat server.
				// You would want them to have some sort of signon message,
				// and you would keep their client in a state of limbo (connected,
				// but not logged on) until them.  I'm trying to keep this example
				// code really simple.
				char nick[ 64 ];
				sprintf( nick, "BraveWarrior%d", 10000 + ( rand() % 100000 ) );

				// Send them a welcome message
				sprintf( temp, "Welcome, stranger.  Thou art known to us for now as '%s'; upon thine command '/nick' we shall know thee otherwise.", nick ); 
				SendStringToClient( pInfo->m_hConn, temp ); 

				// Also send them a list of everybody who is already connected
				if ( m_mapClients.empty() )
				{
					SendStringToClient( pInfo->m_hConn, "Thou art utterly alone." ); 
				}
				else
				{
					sprintf( temp, "%d companions greet you:", (int)m_mapClients.size() ); 
					for ( auto &c: m_mapClients )
						SendStringToClient( pInfo->m_hConn, c.second.m_sNick.c_str() ); 
				}

				// Let everybody else know who they are for now
				sprintf( temp, "Hark!  A stranger hath joined this merry host.  For now we shall call them '%s'", nick ); 
				SendStringToAllClients( temp, pInfo->m_hConn ); 

				// Add them to the client list, using std::map wacky syntax
				m_mapClients[ pInfo->m_hConn ];
				SetClientNick( pInfo->m_hConn, nick );
				SetClientPlayerID( pInfo->m_hConn);
				// if (m_mapClients.size() >= 2){
				// 	beginGame();
				// }
				break;
			}

			case k_ESteamNetworkingConnectionState_Connected:
				// We will get a callback immediately after accepting the connection.
				// Since we are the server, we can ignore this, it's not news to us.
				break;

			default:
				// Silences -Wswitch
				break;
		}
	}

	static ChatServer *s_pCallbackInstance;
	static void SteamNetConnectionStatusChangedCallback( SteamNetConnectionStatusChangedCallback_t *pInfo )
	{
		s_pCallbackInstance->OnSteamNetConnectionStatusChanged( pInfo );
	}

	void PollConnectionStateChanges()
	{
		s_pCallbackInstance = this;
		m_pInterface->RunCallbacks();
	}
};

int input_handling(int argc, const char* argv[]){
	if (argc == 1){
		FatalError( "Please specify port: ./server port_number");
	}
	int nPort = atoi( argv[1] );
	if ( nPort <= 0 || nPort > 65535 )
		FatalError( "Invalid port %d", nPort );
	return nPort;
}


ChatServer *ChatServer::s_pCallbackInstance = nullptr; //what does this do? it seems to be imporant


ChatServer* run_server(int nPort){
	InitSteamDatagramConnectionSockets();
	LocalUserInput_Init();
    ChatServer* server = new ChatServer;
    server->init( nPort );
	return server;
}
