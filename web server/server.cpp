#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include "src/socket.h"
#include "src/Http.h"

// GET cgi-bin/testcgi?name=igor&surname=golovin&mail=igolovin HTTP/1.1
// GET cgi-bin/testcgi?name=igor&surname=golovin&mail=igolovin HTTP/1.1
// GET /index.html HTTP/1.1

std::string receive(int sock, int flags) {
    int size = 0;
    recv(sock, &size, sizeof(int), 0);
    // std::cout << "&&&" << size << std::endl;
    std::vector<char> tmp;
    tmp.resize((int) (size / sizeof(std::vector<char>)));
    recv(sock, &tmp[0], size, flags);
    std::string res(tmp.begin(), tmp.end());
    // res += tmp;
    return res;
}

int main()
{
    int end = 0;
    int connect_err = 0;
    std::string msg;

    ServerSocket server(AF_INET, SOCK_STREAM, 0, AF_INET, 1234, htonl(INADDR_LOOPBACK));
    try { server._bind(); } catch(FatalError e) {
        std::cerr << "SERVER: Fatal Error!\n";
        return 1;
    };

    std::ofstream fout;
    fout.open("log.txt");
    while(1) {
        ConnectedSocket ClientAddress;
        server._listen(1);
        int clSocket = ClientAddress._accept(server.GetSock());
        std::cout << "SERVER: Client " << clSocket << " connected." << std::endl;
        if (clSocket < 0) { 
            connect_err++;
            if(connect_err > 1) {
                std::cerr << "Connect Error!\n";
                return 2;
            }
        } else connect_err = 0;
        std::string h;
        do {
            h = receive(clSocket, 0);
            fout << h << std::endl;
            if(!h.empty() && (h != "exit") && (h != "Close Server")) {
                try{ HttpHeader kek(h); kek.print(); }
                catch(BadMethod m) {
                    std::cout << "BadMethod: " << m.GetErr() << "!!" << std::endl;
                    std::cout << "SERVER: " << "501 Not Implemented" << std::endl;
                }
                catch(BadProtocol p) {
                    std::cout << "BadProtocol: " << p.GetErr() << std::endl;
                    std::cout << "SERVER: " << "501 Not Implemented" << std::endl;
                }
            }
            if(h == "Close Server") {
                std::cout << "SERVER: Server closed." << std::endl;
                end = 1;
                break;
            }
            // if(k > 0) std::cout << msg << std::endl;
        } while((h != "exit") && (h != "Close Server"));
        if(h == "exit") std::cout << "SERVER: Client " << clSocket << " disconnected." << std::endl;
        if(end) break;
    }
    fout.close();
    return 0;
}
