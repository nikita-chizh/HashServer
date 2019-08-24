#pragma once
#include <atomic>
#include <thread>
#include "Utils.h"
#include "TcpServer.h"
#include <atomic>
class Worker {
public:
    Worker()= delete;

    explicit Worker(TcpServer& server):worker(nullptr), _stop(false), _server(server){}

    Worker(Worker& worker):worker(nullptr), _stop(false), _server(worker._server) {}

    Worker(Worker&& worker) noexcept :worker(nullptr), _stop(false), _server(worker._server) {}

    ~Worker(){
        worker->join();
    }

    void start(){
        worker = std::make_unique<std::thread>([this](){
            work();
        });
    }

    void stop(){
        _stop.store(true);
    }

private:
    void work(){
        try{
            _server.bindSocket();
            _server.startListen();
            _server.initLoop();
            while(!_stop){
                _server.process();
            }
            _server.stop();
        }
        catch (const std::exception &error){
            _server.stop();
            return;
        }
    }
private:
    std::unique_ptr<std::thread> worker;
    std::atomic<bool> _stop{false};
    TcpServer &_server;
};
