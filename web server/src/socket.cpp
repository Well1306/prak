#include "socket.h"
#include "Http.h"

void getrequest(std::string &g) { //getline(std::cin, g, "\n\n")
    std::string res;
    char ch;
    while(1) {
        getline(std::cin, res);
        g += res;
        if((ch = getchar()) == '\n') break;
        else {
            g += '\n';
            g += ch;
        }
    }
    g += "\n\n";
}

char* to_str(std::ifstream& file, int &l) {
    int len = 0;
    file.seekg(0, std::ios::end);
    len = file.tellg();
    file.seekg(0, std::ios::beg);
    char *buf = new char[len];
    file.read(buf, len);
    l = len;
    return buf;
}

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
void ServerSocket::work() {
    int end = 0;
    int connect_err = 0;
    int stop = 0;
    int count_req = 0;
    std::string last_mod = GMtime();
    SocketAddress saddr(port);
    try { _bind(saddr); } catch(FatalError e) {
        std::cerr << "SERVER: Fatal Error!\n";
        exit(1);
    };
    std::ofstream fout;
    fout.open(log);
    while(1) {
        SocketAddress caddr;
        _listen(1);
        int clSocket = _accept(caddr);
        ConnectedSocket client(clSocket);
        std::cout << "SERVER: Client " << clSocket << " connected." << std::endl;
        if (clSocket < 0) { 
            connect_err++;
            if(connect_err > 1) {
                std::cerr << "Connect Error!\n";
                exit(2);
            }
        } else connect_err = 0;
        std::string h;
        do {
            client._recv(h);
            if(h.empty()) stop = 1;
            else count_req++;
            if(stop == 1) break;
            fout << "Request " << count_req << ":" << std::endl;
            std::string tmp = h;
            fout << tmp.erase(tmp.length() - 1) << std::endl;
            if(!h.empty() && ((h.find("exit") == std::string::npos) && (h.find("Close Server") == std::string::npos))) {
                try{ 
                    HttpRequest request(h); 
                    HttpResponse response(request, port, name, &last_mod);
                    std::string res = response.to_string();
                    if(*(response.GetCode()) == 200) {
                        int len2;
                        int len1 = res.length();
                        char* buf2 = to_str(response.file, len2);
                        char* buf1 = new char[len1];
                        buf1 = (char*) res.c_str();
                        int size = len2 + len1 + 2;
                        char* buf = new char[size];
                        memcpy(buf, buf1, len1);
                        buf[len1] = '\n';
                        memcpy(buf + len1 + 1, buf2, len2);
                        buf[len1 + len2 + 1] = '\0';
                        send(client.GetSock(), buf, size, 0);
                        std::cout << buf;
                        delete[] buf2;
                        delete[] buf;
                    } else {
                        std::cout << res;
                        client._send(res);
                    }
                }
                catch(BadMethod m) {
                    client._send("Bad method.\n");
                    std::cout << "BadMethod: " << m.GetErr() << "!!" << std::endl;
                    std::cout << "SERVER: " << "404 Not Found" << std::endl;
                }
                catch(BadProtocol p) {
                    client._send("Bad protocol.\n");
                    std::cout << "BadProtocol: " << p.GetErr() << std::endl;
                    std::cout << "SERVER: " << "404 Not Found" << std::endl;
                }
            }
            if(h.find("Close Server") != std::string::npos) {
                std::cout << "SERVER: Server closed." << std::endl;
                end = 1;
                break;
            }
            // if(k > 0) std::cout << msg << std::endl;
        } while((h.find("exit") == std::string::npos) && (h.find("Close Server") == std::string::npos));
        if((h.find("exit") != std::string::npos) || stop) { 
            std::cout << "SERVER: Client " << clSocket << " disconnected." << std::endl;
            close(clSocket); 
            stop = 0;
        }
        if(end) break;
    }
    fout.close();
    shutdown(sock, SHUT_RDWR);
    close(sock);
}

void ClientSocket::work() {
    int end = 0;
    SocketAddress saddr(port);
    ConnectedSocket client(sock);
    if(!client._connect(saddr)) std::cout << "Connected.\n";
    else { std::cout << "Can't connect.\n"; exit(1); }

    std::string g,h;
    HttpResponse result;
    getrequest(g);
    while((g.find("exit") == std::string::npos) && !end) {
        if(!g.empty()) {
            client._send(g);
            client._recv(h);
            std::cout << h << std::endl;
        }
        if(g.find("Close Server") != std::string::npos) {
            std::cout << "Client disconnected." << std::endl;
            end = 1;
            break;
        }
        getrequest(g);
        if(g.find("\n") == 0) g.erase(0, 1);
    }
    if((g.find("exit") != std::string::npos) || end) {
        g = "exit";
        std::vector<char> buf(g.begin(), g.end());
        client._send(g);
        shutdown(sock, SHUT_RDWR);
        close(sock);
    }
}

int ConnectedSocket::_connect(const SocketAddress& saddr) {
    return connect(sock, saddr.GetAddr(), saddr.GetLen());
}
int ConnectedSocket::_send(const std::string &request) {
    return send(sock, request.c_str(), request.length(), 0);
}
// int ConnectedSocket::_send(const File &request) {
//     int size = request.length();
//     char buf[size];
//     int k = 0;
//     std::cout << 1;
//     while((k = read(request.Getfd(), buf, size)) > 0) {
//         std::cout << send(sock, buf, k, 0);
        
//     }
//     return k;
// }
int ConnectedSocket::_send(std::ifstream &request) {
    int len = 0;
    request.seekg(0, std::ios::end);
    len = request.tellg();
    request.seekg(0, std::ios::beg);
    char buf[len];
    request.read(buf, len);
    return send(sock, buf, len, 0);
}

int ConnectedSocket::_recv(std::string& result) {
    std::vector<char> tmp(2048);
    int res = recv(sock, &tmp[0], 2048, 0);
    std::vector<char>::iterator it = tmp.begin();
    std::advance(it, res);
    std::string s(tmp.begin(), it);
    result = s;
    return res;
}

