#include "Http.h"

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

// const int File::length() const {
//     char c;
//     int l;
//     std::cout << 1;
//     while(read(fd, &c, 1)) l++;
//     std::cout << 2;
//     return l;
// }

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
    else if(s == "Content-length") return CONTENTLENGTH;
    else if(s == "Content-type") return CONTENTTYPE;
    else if(s == "Allow") return ALLOW;
    else if(s == "Last-modified") return LASTMODIFIED;
    else return ERROR;
}

HttpHeader::HttpHeader(std::string s) {
    int pos = s.find(':');
    header = s.substr(0, pos);
    s.erase(0, pos + 1);
    value = s;
}

HttpRequest::HttpRequest(std::string request) {
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


    for(int i = 0; i < 8; ++i) 
        h[i] = 0;
    headers.clear();
    pos = s.find("\n");
    s.erase(0, pos + 1);
    pos = s.find("\n");
    while(pos != (int) s.rfind("\n")) { 
        std::string tmp = s.substr(0, pos);
        headers.push_back(tmp);
        s.erase(0, pos + 1);
        pos = s.find("\n");
        for(int i = 0; i < 8; ++i) {
            if((int) GetHeader(tmp) == h[i]) h[i] = 1;
        }
    }
    // std::cout << h << std::endl;
}
void HttpRequest::print() {
    std::cout << "method: " << method << std::endl << "uri: " << http_uri << "prot: " << protocol << std::endl;
    std::vector<std::string> tmp = headers;
    while(!tmp.empty()) {
        std::cout << tmp.front() << std::endl;
        tmp.erase(tmp.begin());
    }
}

HttpResponse::HttpResponse(HttpRequest& r, short p, std::string n, std::string* lm) : port(p), name(n) {
    protocol = "HTTP/" + r.GetProt();
    int error = 0;
    int err404 = 0;
    file.open(r.GetURI().substr(1).c_str(), std::ios::binary);
    int f = open(r.GetURI().substr(1).c_str(), O_RDONLY);
    if(!file) err404 = 1;
    char c;
    int len = 0;
    if(f != - 1) {
        while(read(f, &c, 1))
            len++;
        close(f);
    }
    std::string ftype;
    if(r.GetURI().substr(1).find("jpg") != std::string::npos) ftype = "image/jpeg";
    else if (r.GetURI().substr(1).find("png") != std::string::npos) ftype = "image/apng";
    else ftype = "text/html";
    headers.push_back("Date: " + GMtime());
    headers.push_back("Host: 127.0.0.1:" + std::to_string(port));
    headers.push_back("Server: " + name);
    headers.push_back("Referer: " + r.GetURI());
    headers.push_back("Content-length: " + std::to_string(len));
    headers.push_back("Content-type: " + ftype);
    headers.push_back("Allow: GET, HEAD");
    headers.push_back("Last-modified: " + (*lm));
    *lm = GMtime();
    if(error != 0) {
        code = 501;
        exp = "Not Implemented";
        // headers.clear();
    } else {
        if(err404 != 0) { code = 404; exp = "Not Found"; }
        else { code = 200; exp = "OK"; }
    }
}

const std::string HttpResponse::to_string() const {
    std::string res = protocol + " " + std::to_string(code) + " " + exp + "\n\r";
    std::vector<std::string> tmp = headers;
    // std::cout << tmp.empty() << std::endl;
    while(!tmp.empty()) {
        res += tmp.front() + "\n\r";
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
