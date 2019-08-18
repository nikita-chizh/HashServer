#pragma once
#include <atomic>
#include <thread>
#include "Utils.h"
#include "TcpServer.h"

class Worker {
public:
    Worker()= delete;

    explicit Worker(TcpServer& server):_server(server), worker(nullptr), _stop(false){}

    Worker(Worker& worker):_server(worker._server), worker(nullptr), _stop(false){}

    Worker(Worker&& worker):_server(worker._server), worker(nullptr), _stop(false) {}

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
    std::atomic<bool> _stop = false;
    TcpServer &_server;
};
