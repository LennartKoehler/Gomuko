#include <iostream>
#include "NetworkingServer.hpp"

int main(int argc, const char* argv[]) {
    std::cerr << argc;

    int nPort = input_handling(argc, argv);
    ChatServer* server = run_server(nPort); // TODO make it print the ipaddress to use to connect to it
    
    while (!g_bQuit){
        server->iteration();
        }

    server->shutdown();
    return 0;
}
