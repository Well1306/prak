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
    const std::string GetURI() const { return way; }
    
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

// GET cgi-bin/testcgi?name=igor&surname=golovin&mail=igolovin HTTP/1.1
// GET cgi-bin/testcgi?name=igor&surname=golovin&mail=igolovin HTTP/1.1
// GET /index.html HTTP/1.1

enum headers {
    DATE,
    HOST,
    REFERER,
    USERAGENT,
    SERVER,
    CONTENTLENGTH,
    CONTENTTYPE,
    ALLOW,
    LASTMODIFIED,
    ERROR
};

const headers GetHeader(std::string); 
const std::string GMtime();

class HttpHeader
{
protected:
    std::string header;
    std::string value;
public:
    HttpHeader(std::string s);
    HttpHeader() {}

    void set_value(std::string v) { value = v; }
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

    const std::string GetProt() const { return protocol; }
    const std::vector<std::string> GetHeaders() const { return headers; }
    const std::string GetURI() const { return http_uri.GetURI(); }
    bool empty() {
        return headers.empty();
    }
    void print();

    ~HttpRequest() {
        method.clear();
        http_uri.~URI();
        protocol.clear();
        headers.clear();
    }
};

class HttpResponse
{
private:
    short port;
    std::string name;
    std::string protocol;
    int code;
    std::string exp;
    std::vector<std::string> headers;
public:
    HttpResponse() {}
    HttpResponse(HttpRequest& r, short p, std::string n, std::string *lm);

    void SetProtocol(std::string p) { protocol = p; }
    void SetCode(int c) { code = c; }
    void SetExp(std::string e) { exp = e; }
    void SetHeaders(std::vector<std::string> h){ headers = h; }
    bool empty() {
        return headers.empty();
    }
    void print();
    const std::string to_string() const;

    const std::string GetProtocol() const { return protocol; }
    const int* GetCode() const { return &code; }
    const std::string GetExp() const { return exp; }
    const std::vector<std::string> GetHeaders() const { return headers; }
};

#endif //HTTP_H
