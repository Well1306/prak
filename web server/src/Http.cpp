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

const headers GetHeader(std::string s) {
    if(s == "Date") return DATE;
    else if(s == "Host") return HOST;
    else if(s == "Referer") return REFERER;
    else if(s == "User-agent") return USERAGENT;
    else if(s == "Server") return SERVER;
    else if(s == "Content-lenght") return CONTENTLENGHT;
    else if(s == "Content-type") return CONTENTTYPE;
    else if(s == "Allow") return ALLOW;
    else if(s == "Last-modified") return LASTMODIFIED;
    else return ERROR;
}

const std::string GMtime() {
    struct tm *ptr;
    time_t a;
    a = time(NULL);
    ptr = gmtime(&a);
    std::string t = asctime(ptr);
    t = t.substr(0, t.length() - 1);
    t += " GMT\n";
    t.insert(3, 1, ',');
    int pos = t.find("\n");
    t.erase(pos, 1);
    return t;
}

HttpHeader::HttpHeader(std::string s) {
    int pos = s.find(':');
    header = s.substr(0, pos);
    s.erase(0, pos + 1);
    value = s;
}

HttpRequest::HttpRequest(std::string request) {
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

    // Основные – Date
    // Запрос – Host, Referer, User-agent  
    // Ответ – Server 
    // Сущности – Content-length, Content-type, Allow, Last-modified


    headers.clear();
    pos = s.find("\n");
    s.erase(0, pos + 1);
    pos = s.find("\n");
    while(pos != (int) s.rfind("\n")) { 
        headers.push_back(s.substr(0, pos));
        s.erase(0, pos + 1);
        pos = s.find("\n");
        // std::cout << pos << ',' << s.rfind("\n") << std::endl;
    }
}
void HttpRequest::print() {
    std::cout << "method: " << method << std::endl << "uri: " << http_uri << "prot: " << protocol << std::endl;
    std::vector<std::string> tmp = headers;
    while(!tmp.empty()) {
        std::cout << tmp.front() << std::endl;
        tmp.erase(tmp.begin());
    }
}

HttpResponse::HttpResponse(HttpRequest& r) {
    protocol = "HTTP/" + r.GetProt();
    int error = 0;
    std::vector<std::string> tmp = r.GetHeaders();
    while(!tmp.empty()) {
        std::string t = tmp.front();
        int pos = t.find(":");
        std::string head = t.substr(0, pos);
        std::string value = t.substr(pos + 1);
        tmp.erase(tmp.begin());
        switch(GetHeader(head))
        {
        case DATE:
            headers.push_back("Date: " + GMtime());
            break;
        case HOST:
            headers.push_back("Host: " + value);
            break;
        case REFERER:
            headers.push_back("Referer: " + r.GetURI());
            break;
        case USERAGENT:
            headers.push_back("User-agent: " + value);
            break;
        case SERVER:
            headers.push_back("Server: WEBSERVER");
            break;
        case CONTENTLENGHT:
            headers.push_back("Content-lenght: " + std::to_string(sizeof(r)));
            break;
        case CONTENTTYPE:
            headers.push_back("Content-type: text/html");
            break;
        case ALLOW:
            headers.push_back("Allow: GET, HEAD");
            break;
        case LASTMODIFIED:
            headers.push_back("Last-modified: " + GMtime());
            break;
        default: 
            error++; 
            break;
        }
    }
    if(error != 0) {
        code = 404;
        exp = "Not Found";
        headers.clear();
    } else { code = 200; exp = "OK"; }
}

const std::string HttpResponse::to_string() const {
    std::string res = protocol + " " + std::to_string(code) + " " + exp + "\n";
    std::vector<std::string> tmp = headers;
    while(!tmp.empty()) {
        res += tmp.front() + "\n";
        tmp.erase(tmp.begin());
    }
    return res;
}


void HttpResponse::print() {
        std::cout << protocol << " " << std::to_string(code) << " " << exp << std::endl;
        std::vector<std::string> tmp = headers;
        while(!tmp.empty()) {
            std::cout << tmp.front() << std::endl;
            tmp.erase(tmp.begin());
        }
    }
