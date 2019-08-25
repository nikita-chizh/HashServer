#pragma once
#include <atomic>
#include <thread>
#include "Utils.h"
#include "TcpServer.h"
#include <atomic>
class Worker {
public:
    Worker()= delete;

    explicit Worker(TcpServer& server, std::atomic<bool> &serviseStop):
            worker(nullptr), _stop(false), _serviceStop(serviseStop), _server(server){
        worker = std::make_unique<std::thread>([this](){
            work();
        });
    }

    ~Worker(){
        worker->join();
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
            while(!_stop && !_serviceStop){
                _server.process();
            }
        }
        catch (const std::exception &){
            // some critical event occurred, all service must be stopped
            _serviceStop = true;
            return;
        }
    }
private:
    std::unique_ptr<std::thread> worker;
    std::atomic<bool> _stop{false};
    std::atomic<bool> &_serviceStop;
    TcpServer &_server;
};
