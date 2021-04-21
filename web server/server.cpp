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
// GET /index.html HTTP/1.1



int main()
{
    int end = 0;
    int connect_err = 0;
    int stop = 0;
    std::string name;

    SocketAddress saddr(1233);
    ServerSocket server;
    try { server._bind(saddr); } catch(FatalError e) {
        std::cerr << "SERVER: Fatal Error!\n";
        return 1;
    };

    std::ofstream fout;
    fout.open("log.txt");
    while(1) {
        SocketAddress caddr;
        server._listen(1);
        int clSocket = server._accept(caddr);
        ConnectedSocket client(clSocket);
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
            h.clear();
            client._recv(h);
            if(h.empty()) stop++;
            if(stop > 5) { end = 1; break; }
            if(!h.empty() && ((h.find("exit") == std::string::npos) && (h.find("Close Server") == std::string::npos))) {
                try{ 
                    HttpRequest request(h); 
                    HttpResponse response(request);
                    response.print();
                    client._send(response.to_string());
                }
                catch(BadMethod m) {
                    client._send("BadMethod\n");
                    std::cout << "BadMethod: " << m.GetErr() << "!!" << std::endl;
                    std::cout << "SERVER: " << "501 Not Implemented" << std::endl;
                }
                catch(BadProtocol p) {
                    std::cout << "BadProtocol: " << p.GetErr() << std::endl;
                    std::cout << "SERVER: " << "501 Not Implemented" << std::endl;
                }
            }
            if(h.find("Close Server") != std::string::npos) {
                std::cout << "SERVER: Server closed." << std::endl;
                end = 1;
                break;
            }
            // if(k > 0) std::cout << msg << std::endl;
        } while((h.find("exit") == std::string::npos) && (h.find("Close Server") == std::string::npos));
        if(h.find("exit") != std::string::npos) std::cout << "SERVER: Client " << clSocket << " disconnected." << std::endl;
        if(end) break;
    }
    fout.close();
    return 0;
}
