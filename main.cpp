#include "TcpServer.h"
#include "HashProtocol.h"

#define PORT_NO 3033
int main()
{
    HashProtocol hashProtocol;
    //
    TcpServer::ServerLogic logic;
    logic.accept = [&hashProtocol](int fd){hashProtocol.acceptClient(fd);};
    logic.process = [&hashProtocol](int clientSock, const char* buf, size_t size){
        return hashProtocol.processChunck(clientSock, buf, size);
    };
    logic.close = [&hashProtocol](int clientSock){hashProtocol.closeClient(clientSock);};
    logic.answer = [&hashProtocol](const int clientSock, const char* data, size_t size){
        hashProtocol.writeAnswer(clientSock, data, size);
    };
    TcpServer server(PORT_NO, logic);
    //
    server.bindSocket();
    server.startListen();
    server.startServer();
    return 0;
}

