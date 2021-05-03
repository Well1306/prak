#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <vector>
#include "src/socket.h"
#include "src/Http.h"


int main()
{
    std::cout << 1 << std::endl;
    ServerSocket server(1234, "MyServer", "log.txt");
    server.work();
    return 0;
}
