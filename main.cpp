#include "TcpServer.h"
#include "HashProtocol.h"
#include "Config.h"

int main(int argc, char *argv[])
{
    try {
        if(argc < 2)
            throw std::runtime_error("Not enough arguments");

        auto config = Config(std::string(argv[1]));
        HashProtocol hashProtocol(config.hashFunction);
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
        TcpServer server(10000, logic);
        //
        server.bindSocket();
        server.startListen();
        server.startServer();
        return 0;
    }
    catch(const std::exception &error){
        std::cerr<<error.what();
        return -1;
    }
}

