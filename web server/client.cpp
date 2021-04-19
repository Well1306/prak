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
    SocketAddress saddr(1233);
    ConnectedSocket client;
    std::cout << client._connect(saddr) << std::endl;

    std::string g,h;
    HttpResponse result;
    std::getline(std::cin, g, '%');
    while((g.find("exit") == std::string::npos) && !end) {
        // std::cout << g << std::endl;
        client._send(g);
        // std::cout << client._recv(h) << std::endl;
        // result.print();
        // std::cout << h.empty() << std::endl;
        client._recv(h);
        std::cout << h << std::endl;
        if(g.find("Close Server") != std::string::npos) {
            std::cout << "Client disconnected." << std::endl;
            end = 1;
            break;
        }
        std::getline(std::cin, g, '%');
        if(g.find("\n") == 0) g.erase(0, 1);
    }
    if((g.find("exit") != std::string::npos) || end) {
        g = "exit";
        std::vector<char> buf(g.begin(), g.end());
        client._send(g);
    }

    return 0;
}
