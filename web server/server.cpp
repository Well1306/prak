#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>
#include "src/socket.h"

class BadHttpHeader
{
private:
    std::string m;
public:
    BadHttpHeader(std::string b) : m(b) {}
    std::string GetErr() { return m; }
};

class BadMethod : public BadHttpHeader
{
public:
    BadMethod(std::string b) : BadHttpHeader(b) {}

    // std::string GetErr() { return m; }
};

class BadProtocol : public BadHttpHeader
{
public:
    BadProtocol(std::string b) : BadHttpHeader(b) {}

    // std::string GetErr() { return m; }
};

class URI
{
private:
    std::string way;
    std::vector<std::string> par;
public:
    URI() {}
    URI(std::string uri) {
        std::string s = uri;
        size_t pos = s.find("?");
        way = s.substr(0, pos);
        if(pos != std::string::npos) {
            s.erase(0, pos + 1);
            pos = s.find("&");
            while(pos != std::string::npos) {
                std::string p = s.substr(0, pos + 1);
                par.push_back(p);
                s.erase(0, pos + 1);
                pos = s.find("&");
            }
        }
    }

    URI &operator=(const URI &u) {
        way = u.way;
        par = u.par;
        return *this;
    }
    
    friend std::ostream &operator<<(std::ostream &s, URI &u) {
        s << "way: " << u.way << std::endl;
        std::vector<std::string> tmp(u.par);
        while(!tmp.empty()) {
            s << tmp.front() << std::endl;
            tmp.erase(tmp.begin());
        }
        return s;
    }

    ~URI() {
        way.clear();
        par.clear();
    }
};

class HttpHeader
{
protected:
    std::string method;
    URI http_uri;
    std::string protocol;
public:
    HttpHeader(std::string request) {
        // std::cout << "!!!" << request << std::endl;
        std::string s = request;
        int pos = s.find(" ");
        method = s.substr(0, pos);
        if((method != "GET") && (method != "HEAD")) { 
            BadMethod m(request);
            throw m;
        }
        s.erase(0, pos + 1);


        pos = s.find(" ");
        std::string t = s.substr(0, pos); 
        URI u(t);
        http_uri = u;
        s.erase(0, pos + 1);

        std::cout << "!!" << s << std::endl;
        std::string a = "HTTP/";
        protocol = s.substr(a.length(), 3);
        std::cout << "!!" << protocol << "!!" << std::endl;
        if((protocol != "1.1") && (protocol != "1.0")) { 
            BadProtocol p(request);
            throw p;
        }
    }

    void print() {
        std::cout << "method: " << method << std::endl << "uri: " << http_uri << "prot: " << protocol << std::endl;
    }
    std::string GET(std::string request) { return "501 Not Implemented"; }
};

// GET cgi-bin/testcgi?name=igor&surname=golovin&mail=igolovin HTTP/1.1

class Date : public HttpHeader
{
public:

};

class Host : public HttpHeader
{
public:
  
};


class Refer : public HttpHeader
{
public:
  
};

class UserAgent : public HttpHeader
{
public:
  
};

class Server : public HttpHeader
{
public:
  
};

class ConnectLenght : public HttpHeader
{
public:
  
};

class ConnectType : public HttpHeader
{
public:
  
};

class Allow : public HttpHeader
{
public:
  
};

class LastModified : public HttpHeader
{
public:
  
};

std::string receive(int sock, int flags) {
    int size = 0;
    recv(sock, &size, sizeof(int), 0);
    // std::cout << "&&&" << size << std::endl;
    std::vector<char> tmp;
    tmp.resize((int) (size / sizeof(std::vector<char>)));
    recv(sock, &tmp[0], size, flags);
    std::string res(tmp.begin(), tmp.end());
    // res += tmp;
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
        do {
            h = receive(clSocket, 0);
            fout << h << std::endl;
            if(!h.empty() && (h != "exit") && (h != "Close Server")) {
                std::cout << "SERVER: " << "501 Not Implemented" << std::endl;
                try{ HttpHeader kek(h); kek.print(); }
                catch(BadMethod m) { std::cout << "BadMethod: " << m.GetErr() << "!!" << std::endl; return 1; }
                catch(BadProtocol p) { std::cout << "BadProtocol: " << p.GetErr() << std::endl; return 1; }
            }
            if(h == "Close Server") {
                std::cout << "SERVER: Server closed." << std::endl;
                end = 1;
                break;
            }
            // if(k > 0) std::cout << msg << std::endl;
        } while((h != "exit") && (h != "Close Server"));
        if(h == "exit") std::cout << "SERVER: Client " << clSocket << " disconnected." << std::endl;
        if(end) break;
    }
    fout.close();
    return 0;
}
