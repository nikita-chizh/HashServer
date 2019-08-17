#include "TcpServer.h"
#include "HashProtocol.h"

#define PORT_NO 3033
int main()
{
    HashProtocol hashProtocol;
    TcpServer server(PORT_NO);
    //
    auto acceptor = [&hashProtocol](int fd){hashProtocol.acceptClient(fd);};
    server.setAcceptor(acceptor);
    //
    auto processor = [&hashProtocol](int clientSock, const char* buf, size_t size){
        return hashProtocol.processChunck(clientSock, buf, size);
    };
    server.setDataProcessor(processor);
    //
    auto closer = [&hashProtocol](int clientSock){
        hashProtocol.closeClient(clientSock);
    };
    //

    //
    server.setClose(closer);
    //
    server.bindSocket();
    server.startListen();
    server.startServer();
    return 0;
}

