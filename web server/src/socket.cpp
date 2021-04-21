#include "socket.h"
#include "Http.h"

FatalError::FatalError() {};
FatalError::FatalError(const std::string err) : s(err) {};
std::string FatalError::GetErr() { return s; }
FatalError::~FatalError() {};

SocketAddress::SocketAddress(short port) {
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
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



int ConnectedSocket::_send(const std::string &request){
    std::vector<char> tmp(request.begin(), request.end());
    return send(sock, &tmp[0], request.length(), 0);
}

int ConnectedSocket::_recv(std::string& result) {
    std::vector<char> tmp(2048);
    int res = recv(sock, &tmp[0], 2048, 0);
    std::vector<char>::iterator it = tmp.begin();
    std::advance(it, res);
    std::string s(tmp.begin(), it);
    result = s;
    tmp.clear();
    return res;
}

