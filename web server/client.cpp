#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#include <vector>
#include "src/socket.h"
#include "src/Http.h"

int end = 0;

int main() 
{
    SocketAddress saddr(1234);
    ConnectedSocket client;
    std::cout << client._connect(saddr) << std::endl;

    std::string g;
    std::getline(std::cin, g, '%');
    while((g.find("exit") == std::string::npos) && !end) {
        int pos = g.find("\n\n");
        std::cout << g << std::endl;
        client._send(g);
        if(g.find("Close Server") != std::string::npos) {
            std::cout << "Client disconnected." << std::endl;
            end = 1;
            break;
        }
        std::getline(std::cin, g, '%');
    }
    if((g.find("exit") != std::string::npos) || end) {
        g = "exit";
        std::vector<char> buf(g.begin(), g.end());
        client._send(g);
    }

    return 0;
}
