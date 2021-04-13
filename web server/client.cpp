#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include "src/socket.h"
#include "src/Http.h"

int main() 
{
    SocketAddress saddr(1234);
    ConnectedSocket client;
    std::cout << client._connect(saddr) << std::endl;
    int end = 0;

    std::string g;
    while((g != "exit")) {
        std::getline(std::cin, g);
        client._send(g);
        if(g == "Close Server") {
            std::cout << "Client disconnected." << std::endl;
            end = 1;
            break;
        }
    }
    if((g != "exit") && !end) {
        g = "exit";
        std::vector<char> buf(g.begin(), g.end());
        client._send(g);
    }

    return 0;
}
