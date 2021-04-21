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

void getrequest(std::string &g) { //getline(std::cin, g, "\n\n")
    g.clear();
    std::string res;
    char ch;
    res.clear();
    while(1) {
        getline(std::cin, res);
        g += res;
        if((ch = getchar()) == '\n') break;
        else g += ch;
        g += "\n";
    }
}

int end = 0;

int main() 
{
    SocketAddress saddr(1233);
    ConnectedSocket client;
    std::cout << client._connect(saddr) << std::endl;

    std::string g,h;
    HttpResponse result;
    getrequest(g);
    while((g.find("exit") == std::string::npos) && !end) {
        if(!g.empty()) {
            client._send(g);
            client._recv(h);
            std::cout << h << std::endl;
        }
        if(g.find("Close Server") != std::string::npos) {
            std::cout << "Client disconnected." << std::endl;
            end = 1;
            break;
        }
        getrequest(g);
        if(g.find("\n") == 0) g.erase(0, 1);
    }
    if((g.find("exit") != std::string::npos) || end) {
        g = "exit";
        std::vector<char> buf(g.begin(), g.end());
        client._send(g);
    }

    return 0;
}
