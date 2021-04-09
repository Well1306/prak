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
    struct sockaddr_in sock_struct;
    Socket();
    Socket(int d, int type, int protocol, int f, int p, int a);
public:
    int len();
    virtual int _bind() = 0;
    virtual int _listen(int count) = 0;
    virtual int _accept(int v) = 0;
    virtual int _connect() = 0;
};
class ServerSocket : public Socket
{
public:
    ServerSocket();
    ServerSocket(int d, int type, int protocol, int f, int p, int a);

    int GetSock();
    
    int _bind();
    int _listen(int count);
    int _accept(int v);
    int _connect();
    ~ServerSocket();
};
class ConnectedSocket : public Socket
{
public:
    ConnectedSocket();

    int _bind();
    int _listen(int count);
    int _accept(int v);
    int _connect();
};
class ClientSocket : public Socket
{
public:
    ClientSocket();
    ClientSocket(int d, int type, int protocol, int f, int p, int a);

    int _bind();
    int _listen(int count);
    int _accept(int v);
    int _connect();
};

#endif //SOCKET_H
