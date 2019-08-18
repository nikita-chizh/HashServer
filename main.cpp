#include "TcpServer.h"
#include "HashProtocol.h"
#include "Config.h"
#include "Worker.h"

void worker(const Config &config)
{
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
}


int main(int argc, char *argv[])
{
    try {
        if(argc < 2)
            throw std::runtime_error("Not enough arguments");

        auto config = Config(std::string(argv[1]));
        auto numberOfAcceptors = config.numberOfAcceptors;
        //
        std::vector<HashProtocol> protocolHandlers;
        std::vector<TcpServer> servers;
        std::vector<Worker> workers;

        protocolHandlers.reserve(numberOfAcceptors);
        servers.reserve(numberOfAcceptors);
        workers.reserve(numberOfAcceptors);
        for(int i=0; i < numberOfAcceptors; ++i)
            protocolHandlers.emplace_back(config.hashFunction);

        for(int i=0; i < numberOfAcceptors; ++i){
            TcpServer::ServerLogic logic;
            logic.accept = [&protocolHandlers, i](int fd){protocolHandlers[i].acceptClient(fd);};
            logic.process = [&protocolHandlers, i](int clientSock, const char* buf, size_t size){
                return protocolHandlers[i].processChunck(clientSock, buf, size);
            };
            logic.close = [&protocolHandlers, i](int clientSock){protocolHandlers[i].closeClient(clientSock);};
            logic.answer = [&protocolHandlers, i](const int clientSock, const char* data, size_t size){
                protocolHandlers[i].writeAnswer(clientSock, data, size);
            };
            servers.emplace_back(config.port, logic);
            // START
            servers[i].bindSocket();
            servers[i].startListen();
            servers[i].initLoop();
        }
        for(int i=0; i < numberOfAcceptors; ++i){
            workers.emplace_back(servers[i]);
            workers[i].start();
        }


        return 0;
    }
    catch(const std::exception &error){
        std::cerr<<error.what();
        return -1;
    }
}

