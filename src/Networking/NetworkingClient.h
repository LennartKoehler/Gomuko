#pragma once

#include "NetworkingEvents.h"

/////////////////////////////////////////////////////////////////////////////
//
// ChatClient, client and server are almost the same, except server can have conn to multiple clients
//
/////////////////////////////////////////////////////////////////////////////

class ChatClient
{
public:

	void init( const SteamNetworkingIPAddr &serverAddr )
	{

		// Select instance to use.  For now we'll always use the default.
		m_pInterface = SteamNetworkingSockets();

		// Start connecting
		char szAddr[ SteamNetworkingIPAddr::k_cchMaxString ];
		serverAddr.ToString( szAddr, sizeof(szAddr), true );
		Printf( "Connecting to chat server at %s", szAddr );
		SteamNetworkingConfigValue_t opt;
		opt.SetPtr( k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback );
		m_hConnection = m_pInterface->ConnectByIPAddress( serverAddr, 1, &opt );
		if ( m_hConnection == k_HSteamNetConnection_Invalid )
			FatalError( "Failed to create connection" );

	}
	void iteration(){
		PollIncomingMessages();
		PollConnectionStateChanges();
		PollLocalUserInput();
	}

	void sendToServer(Package& package){
		std::vector<uint8_t> buffer = package.serialize();
		// for (int i = 0; i < 8 && i < buffer.size(); ++i)
		// 	std::cerr << (int)buffer[i] << " ";
		// int* test = new int(5);
		m_pInterface->SendMessageToConnection(m_hConnection, buffer.data(), buffer.size(), k_nSteamNetworkingSend_Reliable, nullptr);
	}


	void shutdown(){
		ShutdownSteamDatagramConnectionSockets();
		NukeProcess(0);
	}


private:

	HSteamNetConnection m_hConnection;
	ISteamNetworkingSockets *m_pInterface;

	void PollIncomingMessages()
	{
		while ( !g_bQuit )
		{
			ISteamNetworkingMessage *pIncomingMsg = nullptr;
			int numMsgs = m_pInterface->ReceiveMessagesOnConnection( m_hConnection, &pIncomingMsg, 1 );
			if ( numMsgs == 0 )
				break;
			if ( numMsgs < 0 )
				FatalError( "Error checking for messages" );


			Package* package = steamMessageToPackage(pIncomingMsg); // IMPORTANT this is the interface
			packageToSDLEvent(package);
			std::cerr << "recieved new package" << std::endl;
			pIncomingMsg->Release();

		}
	}



	void PollLocalUserInput()
	{
		std::string cmd;
		while ( !g_bQuit && LocalUserInput_GetNext( cmd ))
		{

			// Check for known commands
			if ( strcmp( cmd.c_str(), "/quit" ) == 0 )
			{
				g_bQuit = true;
				Printf( "Disconnecting from chat server" );

				// Close the connection gracefully.
				// We use linger mode to ask for any remaining reliable data
				// to be flushed out.  But remember this is an application
				// protocol on UDP.  See ShutdownSteamDatagramConnectionSockets
				m_pInterface->CloseConnection( m_hConnection, 0, "Goodbye", true );
				break;
			}
		

			// Anything else, just send it to the server and let them parse it
			//m_pInterface->SendMessageToConnection( m_hConnection, cmd.c_str(), (uint32)cmd.length(), k_nSteamNetworkingSend_Reliable, nullptr );
		}
	}

	void OnSteamNetConnectionStatusChanged( SteamNetConnectionStatusChangedCallback_t *pInfo )
	{
		assert( pInfo->m_hConn == m_hConnection || m_hConnection == k_HSteamNetConnection_Invalid );

		// What's the state of the connection?
		switch ( pInfo->m_info.m_eState )
		{
			case k_ESteamNetworkingConnectionState_None:
				// NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
				break;

			case k_ESteamNetworkingConnectionState_ClosedByPeer:
			case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
			{
				g_bQuit = true;

				// Print an appropriate message
				if ( pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting )
				{
					// Note: we could distinguish between a timeout, a rejected connection,
					// or some other transport problem.
					Printf( "We sought the remote host, yet our efforts were met with defeat.  (%s)", pInfo->m_info.m_szEndDebug );
				}
				else if ( pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally )
				{
					Printf( "Alas, troubles beset us; we have lost contact with the host.  (%s)", pInfo->m_info.m_szEndDebug );
				}
				else
				{
					// NOTE: We could check the reason code for a normal disconnection
					Printf( "The host hath bidden us farewell.  (%s)", pInfo->m_info.m_szEndDebug );
				}

				// Clean up the connection.  This is important!
				// The connection is "closed" in the network sense, but
				// it has not been destroyed.  We must close it on our end, too
				// to finish up.  The reason information do not matter in this case,
				// and we cannot linger because it's already closed on the other end,
				// so we just pass 0's.
				m_pInterface->CloseConnection( pInfo->m_hConn, 0, nullptr, false );
				m_hConnection = k_HSteamNetConnection_Invalid;
				break;
			}

			case k_ESteamNetworkingConnectionState_Connecting:
				// We will get this callback when we start connecting.
				// We can ignore this.
				break;

			case k_ESteamNetworkingConnectionState_Connected:
				Printf( "Connected to server OK" );
				break;

			default:
				// Silences -Wswitch
				break;
		}
	}

	static ChatClient *s_pCallbackInstance;
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




static bool get_address(const char* address, SteamNetworkingIPAddr& addrServer){
	addrServer.Clear();

	if ( !addrServer.ParseString( address ) )
		return false;
	if ( addrServer.m_port == 0 )
		addrServer.m_port = DEFAULT_SERVER_PORT;
	return true;
}

// static SteamNetworkingIPAddr input_handling(int argc, const char* argv[]){
// 	if (argc == 1){
// 		FatalError( "Please specify server address: ./client server_address");
// 	}
// 	SteamNetworkingIPAddr addrServer = get_address(argv[1]);
// 	return addrServer;
// }

ChatClient *ChatClient::s_pCallbackInstance = nullptr; //what does this do? it seems to be imporant


ChatClient* run_client(SteamNetworkingIPAddr addrServer){
	InitSteamDatagramConnectionSockets();
	LocalUserInput_Init();
    ChatClient* client = new ChatClient;
    client->init( addrServer );
	return client;
}
