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
        std::function<void(const int &clientSock, std::vector<char> &&data)> answer;
    };

    TcpServer() = delete;

    ~TcpServer();

    explicit TcpServer(const uint16_t &port, const ServerLogic &logic);

    void bindSocket();
    void startListen();
    void initLoop();
    void process();
    void stop();
private:
    static void acceptConnection(struct ev_loop *loop, struct ev_io *acceptIO, int revents);

    static void readData(struct ev_loop *loop, struct ev_io *watcher, int revents);

private:
    uint16_t _port;
    ServerLogic _logic;
    SimpleLoop _loop;
    sockaddr_in _srvAddr;
    int _serverSocket;
    ev_io _acceptIO;
    const int _clientBacklog = 32;
};
