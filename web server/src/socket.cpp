#include "socket.h"

FatalError::FatalError() {};
FatalError::FatalError(const std::string err) : s(err) {};
FatalError::~FatalError() {};

Socket::Socket() {};
Socket::Socket(int d, int type, int protocol, int f, int p, int a) {
    sock = socket(d, type, protocol);
    sock_struct.sin_family = f;
    sock_struct.sin_port = p;
    sock_struct.sin_addr.s_addr = a;
}; 

int Socket::len() { return sizeof(sock_struct); }

ServerSocket::ServerSocket() {};
ServerSocket::ServerSocket(int d, int type, int protocol, int f, int p, int a) : Socket(d, type, protocol, f, p, a) {};
ServerSocket::~ServerSocket() {};

int ServerSocket::GetSock() { return sock; }

int ServerSocket::_bind() {
    int k;
    if((k = bind(sock, (struct sockaddr *) &sock_struct, Socket::len())) < 0) { 
        FatalError e("bind");
        throw e; 
    }
    return k;
}
int ServerSocket::_listen(int count) {
    return listen(sock, count);
}
int ServerSocket::_accept(int v) { return 0; }
int ServerSocket::_connect() { return connect(sock, (struct sockaddr *) &sock_struct, Socket::len()); }

ConnectedSocket::ConnectedSocket() {};

int ConnectedSocket::_bind() { return 0; }
int ConnectedSocket::_listen(int count) { return 0; }
int ConnectedSocket::_accept(int v) {
    socklen_t k = Socket::len();
    return accept(v, (struct sockaddr *) &sock_struct, &k);
}
int ConnectedSocket::_connect() { return 0; }

ClientSocket::ClientSocket() {};
ClientSocket::ClientSocket(int d, int type, int protocol, int f, int p, int a) : Socket(d, type, protocol, f, p, a) {}

int ClientSocket::_bind() { return 0; }
int ClientSocket::_listen(int count) { return 0; }
int ClientSocket::_accept(int v) { return 0; }
int ClientSocket::_connect() {
    return connect(sock, (struct sockaddr *) &sock_struct, Socket::len());
}
