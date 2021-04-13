#include "Http.h"

URI::URI() {}
URI::URI(std::string uri) {
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
URI &URI::operator=(const URI &u) {
    way = u.way;
    par = u.par;
    return *this;
}
URI::~URI() {
    way.clear();
    par.clear();
}

HttpHeader::HttpHeader(std::string request) {
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
    URI u = t;
    http_uri = u;
    s.erase(0, pos + 1);

    std::string a = "HTTP/";
    protocol = s.substr(a.length(), 3);
    if((protocol != "1.1") && (protocol != "1.0")) { 
        BadProtocol p(request);
        throw p;
    }
}
void HttpHeader::print() {
    std::cout << "method: " << method << std::endl << "uri: " << http_uri << "prot: " << protocol << std::endl;
}
