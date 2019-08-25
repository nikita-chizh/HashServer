#include "TcpServer.h"
#include "HashProtocol.h"
#include "Config.h"
#include "Worker.h"
#include "ThreadPool.h"

std::vector<std::unique_ptr<Worker>> workers;
std::atomic<bool> stop;

void sig_handler(int signo){
    if (signo == SIGINT){
        stop.store(true);
    }
}

int main(int argc, char *argv[])
{
    try {
        if(argc < 2)
            throw std::runtime_error("Not enough arguments");

        if (signal(SIGINT, sig_handler) == SIG_ERR)
            throw std::runtime_error("Cannot set SIGINT handler");
        stop = false;
        auto config = Config(std::string(argv[1]));
        auto numberOfAcceptors = config.numberOfAcceptors;
        //
        std::vector<HashProtocol> protocolHandlers;
        std::vector<TcpServer> servers;
        ThreadPool threadPool(config.numberOfProcessors, stop);

        protocolHandlers.reserve(numberOfAcceptors);
        servers.reserve(numberOfAcceptors);
        //
        for(int i=0; i < numberOfAcceptors; ++i)
            protocolHandlers.emplace_back(config.hashFunction, threadPool);

        for(int i=0; i < numberOfAcceptors; ++i){
            TcpServer::ServerLogic logic;
            logic.accept = [&protocolHandlers, i](int fd){protocolHandlers[i].acceptClient(fd);};
            logic.process = [&protocolHandlers, i](int clientSock, const char* buf, size_t size){
                return protocolHandlers[i].processChunk(clientSock, buf, size);
            };
            logic.close = [&protocolHandlers, i](int clientSock){protocolHandlers[i].closeClient(clientSock);};
            logic.answer = [&protocolHandlers, i](const int &clientSock, std::vector<char> &&data){
                protocolHandlers[i].writeAsyncAnswer(clientSock, std::move(data));
            };
            servers.emplace_back(config.port, logic);
        }
        for(int i=0; i < numberOfAcceptors; ++i){
            workers.push_back(std::make_unique<Worker>(servers[i], stop));
        }
        while(!stop){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout<<"SERVER IS STOPPING"<<std::endl;
        return 0;
    }
    catch(const std::exception &error){
        std::cerr<<error.what();
        return -1;
    }
}

