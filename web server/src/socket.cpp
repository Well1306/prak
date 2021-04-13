#include "socket.h"

FatalError::FatalError() {};
FatalError::FatalError(const std::string err) : s(err) {};
std::string FatalError::GetErr() { return s; }
FatalError::~FatalError() {};

SocketAddress::SocketAddress(short port) {
    saddr.sin_family = AF_INET;
    saddr.sin_port = port;
    saddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

int ServerSocket::_bind(const SocketAddress& addr) {
    int k;
    if((k = bind(sock, addr.GetAddr(), addr.GetLen())) < 0) { 
        FatalError e("bind");
        throw e; 
    }
    return k;
}

int ServerSocket::_accept(SocketAddress& addr) {
    socklen_t k = addr.GetLen();
    return accept(sock, addr.GetAddr(), &k);
}

int ServerSocket::_listen(int count) {
    return listen(sock, count);
}

int ConnectedSocket::_connect(const SocketAddress& saddr) {
    return connect(sock, saddr.GetAddr(), saddr.GetLen());
}


int ConnectedSocket::_send(const std::vector<char>& request) {
    int size = request.size() * sizeof(request);
    if(send(sock, &size, sizeof(int), 0) == -1) return -1;
    return send(sock, request.data(), request.size(), 0);
}

int ConnectedSocket::_send(const std::string& request) {
    std::vector<char> buf(request.begin(), request.end());
    return _send(buf);
}

int ConnectedSocket::_recv(std::vector<char>& result) {
    int size = 0;
    recv(sock, &size, sizeof(int), 0);
    result.clear();
    result.resize((int) (size / sizeof(std::vector<char>)));
    return recv(sock, &result[0], size, 0);
}

int ConnectedSocket::_recv(std::string& result) {
    int size = 0;
    recv(sock, &size, sizeof(int), 0);
    std::vector<char> tmp;
    tmp.resize((int) (size / sizeof(std::vector<char>)));
    int res = recv(sock, &tmp[0], size, 0);
    std::string g(tmp.begin(), tmp.end());
    result = g;
    g.clear();
    return res;
}
