#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include "src/socket.h"

int main() 
{
    ServerSocket server(AF_INET, SOCK_STREAM, 0, AF_INET, 1234, htonl(INADDR_LOOPBACK));
    std::cout << server._connect() << std::endl;
    int end = 0;

    std::string g;
    while((g != "exit")) {
        std::getline(std::cin, g);
        std::vector<char> buf(g.begin(), g.end());
        // std::cout << buf << std::endl;
        int size = buf.size() * sizeof(buf);
        send(server.GetSock(), &size, sizeof(int), 0);
        // std::cout << "***" << size << std::endl;
        send(server.GetSock(), buf.data(), buf.size(), 0);
        if(g == "Close Server") {
            std::cout << "Client disconnected." << std::endl;
            end = 1;
            break;
        }
    }
    if((g != "exit") && !end) {
        g = "exit";
        std::vector<char> buf(g.begin(), g.end());
        send(server.GetSock(), buf.data(), buf.size(), 0);
    }

    return 0;
}
