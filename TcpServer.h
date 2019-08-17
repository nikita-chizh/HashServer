#pragma once
#include <cstdint>
#include <netinet/in.h>
//
#include <stdexcept>
#include <memory>
//
#include "Utils.h"
#define BUFFER_SIZE 1024

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
    void setReader(std::function<void(int, const char*, size_t)> &readLogic)
    {
        _readLogic = readLogic;
    }

private:
    static void acceptConnection(struct ev_loop *loop, struct ev_io *watcher, int revents);
    static void readData(struct ev_loop *loop, struct ev_io *watcher, int revents);

private:
    SimpleLoop loop;
    sockaddr_in _srvAddr;
    int _serverSocket;
    ev_io _acceptIO;
    const int _clientBacklog = 32;
    //
    std::function<void(int)> _acceptLogic;
    std::function<void(int, const char*, size_t)> _readLogic;
    //
    std::atomic<bool> _stop;
};
