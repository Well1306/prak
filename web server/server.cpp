#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "src/socket.h"

class HttpHeader
{
protected:
    std::string name;
    std::string value;
};

class Date : public HttpHeader
{

};

class Host : public HttpHeader
{

};


class Refer : public HttpHeader
{

};

class UserAgent : public HttpHeader
{

};

class Server : public HttpHeader
{

};

class ConnectLenght : public HttpHeader
{

};

class ConnectType : public HttpHeader
{

};

class Allow : public HttpHeader
{

};

class LastModified : public HttpHeader
{

};

std::string receive(int sock, int size, int flags) {
    char tmp[size];
    std::string res;
    recv(sock, tmp, size, flags);
    res += tmp;
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
        while((h != "exit") && (h != "Close Server")) {
            // int k;
            // k = recv(clSocket, &msg, 256, 0) > 0;
            h = receive(clSocket, 256, 0);
            fout << h << std::endl;
            if(!h.empty()) std::cout << "SERVER: " << "501 Not Implemented" << std::endl;
            if(h == "Close") {
                h = receive(clSocket, 256, 0);
                if(h == "Server") {
                    std::cout << "SERVER: Server closed." << std::endl;
                    end = 1;
                    break;
                }
            }
            // if(k > 0) std::cout << msg << std::endl;
        }
        if(end) break;
    }
    fout.close();
    return 0;
}
