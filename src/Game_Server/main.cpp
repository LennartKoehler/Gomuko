#include <iostream>
#include "NetworkingServer.hpp"



int main(int argc, const char* argv[]) {
    int nPort = input_handling(argc, argv);
    ChatServer* server = run_server(nPort);
    
    while (!g_bQuit){
        server->iteration();
        }


    server->shutdown();
    return 0;
}
