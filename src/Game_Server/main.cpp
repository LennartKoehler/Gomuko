#include "../Game_Common/GameState.hpp"
#include <iostream>
#include "NetworkingServer.hpp"



int main(int argc, const char* argv[]) {
    GameState gameState(20, 20, 1, 5);
    int nPort = input_handling(argc, argv);
    ChatServer* server = run_server(nPort);

    while (!g_bQuit){
        server->iteration();

    }


    server->shutdown();
    
    return 0;
}
