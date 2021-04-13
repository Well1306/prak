#ifndef HTTP_H
#define HTTP_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <vector>

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
    URI();
    URI(std::string uri);
    URI &operator=(const URI &u);
    
    friend std::ostream &operator<<(std::ostream &s, URI &u) {
        s << "\nway: \t" << u.way << std::endl;
        std::vector<std::string> tmp(u.par);
        while(!tmp.empty()) {
            s << "\t" << tmp.front() << std::endl;
            tmp.erase(tmp.begin());
        }
        return s;
    }
    ~URI();
};

class HttpHeader
{
protected:
    std::string method;
    URI http_uri;
    std::string protocol;
public:
    HttpHeader(std::string request);

    void print();
    std::string GET(std::string request) { return "501 Not Implemented"; }
};

// GET cgi-bin/testcgi?name=igor&surname=golovin&mail=igolovin HTTP/1.1
// GET cgi-bin/testcgi?name=igor&surname=golovin&mail=igolovin HTTP/1.1
// GET /index.html HTTP/1.1

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

#endif //HTTP_H
