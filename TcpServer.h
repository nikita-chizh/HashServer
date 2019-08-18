#pragma once
#include <cstdint>
#include <netinet/in.h>
//
#include "Utils.h"

class TcpServer {
public:
    struct ServerLogic{
        std::function<void(int)> accept;
        std::function<void(int)> close;
        std::function<ProcessRes(const int, const char*, const size_t)> process;
        std::function<void(const int clientSock, const char* data, size_t size)> answer;
    };

    TcpServer() = delete;
    ~TcpServer();

    explicit TcpServer(const uint16_t port, const ServerLogic &logic);
    void bindSocket();
    void startListen();
    void startServer();
    void stopServer();

private:
    static void acceptConnection(struct ev_loop *loop, struct ev_io *acceptIO, int revents);

    static void readData(struct ev_loop *loop, struct ev_io *watcher, int revents);

private:
    ServerLogic _logic;
    SimpleLoop _loop;
    sockaddr_in _srvAddr;
    int _serverSocket;
    ev_io _acceptIO;
    const int _clientBacklog = 32;
    //
    std::atomic<bool> _stop{false};
};
