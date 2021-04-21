#include "socket.h"
#include "Http.h"


void getrequest(std::string &g) { //getline(std::cin, g, "\n\n")
    g.clear();
    std::string res;
    char ch;
    res.clear();
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
            h.clear();
            client._recv(h);
            if(h.empty()) stop++;
            else count_req++;
            if(stop == 1) { end = 1; break; }
            fout << "Request " << count_req << ":" << std::endl;
            std::string tmp = h;
            fout << tmp.erase(tmp.length() - 1) << std::endl;
            tmp.clear();
            if(!h.empty() && ((h.find("exit") == std::string::npos) && (h.find("Close Server") == std::string::npos))) {
                try{ 
                    HttpRequest request(h); 
                    std::cout << request.empty() << std::endl;
                    HttpResponse response(request, port, name, &last_mod);
                    std::cout << response.empty() << std::endl;
                    response.print();
                    client._send(response.to_string());
                }
                catch(BadMethod m) {
                    client._send("Bad method.\n");
                    std::cout << "BadMethod: " << m.GetErr() << "!!" << std::endl;
                    std::cout << "SERVER: " << "501 Not Implemented" << std::endl;
                }
                catch(BadProtocol p) {
                    client._send("Bad protocol.\n");
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
    }
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

