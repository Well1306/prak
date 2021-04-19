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


int ConnectedSocket::_send(const std::vector<char>& request) {
    int size = request.size() * sizeof(request);
    if(send(sock, &size, sizeof(int), 0) == -1) return -1;
    return send(sock, request.data(), request.size(), 0);
}

int ConnectedSocket::_send(const std::string& request) {
    std::vector<char> buf(request.begin(), request.end());
    return _send(buf);
}

int ConnectedSocket::_send(const HttpResponse& request) {
    std::string h = request.to_string();
    return _send(h);
    // int res = send(sock, request.GetCode(), sizeof(int), 0);
    // std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << _send(request.GetProtocol()) << std::endl;
    // res += _send(request.GetExp());
    // std::vector<std::string> tmp = request.GetHeaders();
    // int size = tmp.size();
    // std::cout << "kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk" << size << std::endl;
    // res += send(sock, &size, sizeof(int), 0);
    // while(!tmp.empty()) {
    //     res += _send(tmp.front());
    //     tmp.erase(tmp.begin());
    // }
    // return res;
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

int ConnectedSocket::_recv(HttpResponse& result) {
    return 0;
    // int res = 0;
    // int c;
    // res = recv(sock, &c, sizeof(int), 0);
    // result.SetCode(c);
    // std::cout << "\t\t\t" << c << std::endl;
    // std::string tmp;
    // std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<  _recv(tmp) << std::endl; //protocol
    // std::cout << "\t\t\t" << tmp << std::endl;
    // result.SetProtocol(tmp);
    // tmp.clear();
    // res += _recv(tmp); //exp
    // std::cout << "\t\t\t" << tmp << std::endl;
    // tmp.clear();
    // result.SetExp(tmp);
    // tmp.clear();
    // std::vector<std::string> t;
    // int k = 0;
    // std::cout << recv(sock, &k, sizeof(int), 0) << std::endl;
    // std::cout << "kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk" << k << std::endl;
    // for(int i = 0; i < k; ++i) {
    //     res += _recv(tmp);
    //     t.push_back(tmp);
    // }
    // result.SetHeaders(t);
    // return res;
}
