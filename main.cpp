#include "TcpServer.h"

#define PORT_NO 3033
int main()
{
    TcpServer server(PORT_NO);
    server.bindSocket();
    server.startListen();
    server.startServer();
    return 0;
}

