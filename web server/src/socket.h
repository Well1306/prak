#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include "Http.h"

class FatalError
{
private:
    std::string s;
public:
    FatalError();
    FatalError(const std::string err);

    std::string GetErr();
    
    ~FatalError();
};

class Socket
{
protected:
    int sock;
    Socket() : sock(socket(AF_INET, SOCK_STREAM, 0)) {}
    explicit Socket(int s) : sock(s) {}
public:
    const int GetSock() const { return sock; }
};

class SocketAddress
{
private:
    struct sockaddr_in saddr;
public:
    SocketAddress() {};
    SocketAddress(short);
    const struct sockaddr *GetAddr() const { return (sockaddr*) &saddr; }
    struct sockaddr *GetAddr() { return (sockaddr*) &saddr; }
    const socklen_t GetLen() const { return sizeof(saddr); }
};

class ServerSocket : public Socket
{
public:
    ServerSocket() : Socket() {};
    int _bind(const SocketAddress&);
    int _accept(SocketAddress&);
    int _listen(int);
};

class ConnectedSocket : public Socket
{
public:
    ConnectedSocket() : Socket() {}
    explicit ConnectedSocket(int s) : Socket(s) {}
    int _connect(const SocketAddress&);
    int _send(const std::string&);
    int _recv(std::string&);
};

#endif //SOCKET_H
