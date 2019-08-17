#pragma once
#include <cstdint>
#include <netinet/in.h>
//
#include <stdexcept>
#include <memory>
//
#include "Utils.h"

class TcpServer {
public:
    TcpServer() = delete;
    explicit TcpServer(uint16_t port);
    void bindSocket();
    void startListen();
    void startServer();

    void setAcceptor(const std::function<void(int)> &acceptLogic)
    {
        _acceptLogic = acceptLogic;
    }

    void setDataProcessor(const std::function<ProcessRes(int, const char*, size_t)> &processLogic)
    {
        _processLogic = processLogic;
    }

    void setClose(const std::function<void(int)> &closeLogic)
    {
        _closeLogic = closeLogic;
    }


private:
    static void acceptConnection(struct ev_loop *loop, struct ev_io *acceptIO, int revents);

    static void readData(struct ev_loop *loop, struct ev_io *watcher, int revents);

private:
    SimpleLoop loop;
    sockaddr_in _srvAddr;
    int _serverSocket;
    ev_io _acceptIO;
    const int _clientBacklog = 32;
    //
    std::function<void(int)> _acceptLogic;
    std::function<void(int)> _closeLogic;
    std::function<ProcessRes(int, const char*, size_t)> _processLogic;
    //
    std::atomic<bool> _stop;
};
