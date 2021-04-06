#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "src/socket.h"

int main() 
{
    ServerSocket server(AF_INET, SOCK_STREAM, 0, AF_INET, 1234, htonl(INADDR_LOOPBACK));
    std::cout << server._connect() << std::endl;
    int end = 0;

    std::string g;
    while((g != "exit")) {
        std::cin >> g;
        char buf[sizeof(g)];
        strcpy(buf, g.c_str());
        send(server.GetSock(), buf, sizeof(buf), 0);
        if(g == "Close") {
            std::cin >> g;
            char buf[sizeof(g)];
            strcpy(buf, g.c_str());
            send(server.GetSock(), buf, sizeof(buf), 0);
            if(g == "Server") {
                std::cout << "Client disconnected." << std::endl;
                end = 1;
                break;
            }
        }
    }
    if((g != "exit") && !end) {
        g = "exit";
        char buf[sizeof(g)];
        strcpy(buf, g.c_str());
        send(server.GetSock(), buf, sizeof(buf), 0);
    }

    return 0;
}
