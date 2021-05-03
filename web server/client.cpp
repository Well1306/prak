#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>
#include <vector>
#include "src/socket.h"
#include "src/Http.h"

// GET cgi-bin/testcgi?name=igor&surname=golovin&mail=igolovin HTTP/1.1
// GET /index.html HTTP/1.1

int end = 0;

int main() 
{
    ClientSocket client(1234, "Client");
    client.work();
    return 0;
}
