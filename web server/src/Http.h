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

};

class BadProtocol : public BadHttpHeader
{
public:
    BadProtocol(std::string b) : BadHttpHeader(b) {}

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

class HttpRequest
{
protected:
    std::string method;
    URI http_uri;
    std::string protocol;
    std::vector<std::string> headers;
public:
    HttpRequest(std::string request);

    void print();
    virtual std::string GET(std::string request) { return "501 Not Implemented"; }
};

// GET cgi-bin/testcgi?name=igor&surname=golovin&mail=igolovin HTTP/1.1
// GET cgi-bin/testcgi?name=igor&surname=golovin&mail=igolovin HTTP/1.1
// GET /index.html HTTP/1.1

class HttpHeader
{
protected:
    std::string header;
    std::string value;
public:
    HttpHeader(std::string s) {
        int pos = s.find(':');
        header = s.substr(0, pos);
        s.erase(0, pos + 1);
        value = s;
    }
    HttpHeader() {}

    void set_value(std::string v) { value = v; }
};

class Date : public HttpHeader
{
public:
    Date() : HttpHeader("Date") {}
    const std::string GMtime() const;
};

class Host : public HttpHeader
{
public:
    Host() : HttpHeader("Host") {}
};


class Refer : public HttpHeader
{
public:
    Refer() : HttpHeader("Refer") {}
};

class UserAgent : public HttpHeader
{
public:
    UserAgent() : HttpHeader("User-Agent") {}
};

class Server : public HttpHeader
{
public:
    Server() : HttpHeader("Server") {}
};

class ConnectLenght : public HttpHeader
{
public:
    ConnectLenght() : HttpHeader("Connect-Lenght") {}
};

class ConnectType : public HttpHeader
{
public:
    ConnectType() : HttpHeader("Connect-Type") {}
};

class Allow : public HttpHeader
{
public:
    Allow() : HttpHeader("Allow") {}
};

class LastModified : public HttpHeader
{
public:
    LastModified() : HttpHeader("Last-Modified") {}
};

#endif //HTTP_H
