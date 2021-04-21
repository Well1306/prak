#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#include <vector>
#include "src/socket.h"
#include "src/Http.h"

int end = 0;

int main() 
{
    ClientSocket client(1233, "Client");
    client.work();
    return 0;
}
