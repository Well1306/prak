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
#include <fcntl.h>
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
private:
    short port;
    std::string name;
    std::string log;
    char** GetEnvp();
public:
    ServerSocket(short p, std::string n, std::string l) : Socket(), port(p), name(n), log(l) {};
    int _bind(const SocketAddress&);
    int _accept(SocketAddress& addr) {
        socklen_t k = addr.GetLen();
        return accept(sock, addr.GetAddr(), &k);
    }
    int _listen(int count) {
        return listen(sock, count);
    }
    void work();
};

class ClientSocket : public Socket {
private:
    short port;
    std::string name;
public:
    ClientSocket(short p, std::string n) : Socket(), port(p), name(n) {};
    void work();
};

class ConnectedSocket : public Socket
{
public:
    ConnectedSocket() : Socket() {}
    explicit ConnectedSocket(int s) : Socket(s) {}
    int _connect(const SocketAddress&);
    int _send(const std::string&);
    // int _send(const File&);
    int _send(std::ifstream&);
    int _recv(std::string&);
};

#endif //SOCKET_H
