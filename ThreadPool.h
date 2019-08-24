#pragma once

#include <vector>
#include <atomic>
#include <thread>
#include "ThreadSaveQueue.h"

using ThreadPoolFunc = std::function<void()>;
class ThreadPool
{
public:
    ThreadPool(const int threadCount, std::atomic<bool> &serviseStop)
    :_threadCount(threadCount), _stop(false), _serviceStop(serviseStop){
        try{
            for(int i=0; i<_threadCount; ++i){
                _threads.emplace_back(&ThreadPool::worker_thread, this);
            }
        }
        catch(const std::exception &){
            _serviceStop=true;
        }
    }

    ~ThreadPool(){
        for(auto & t: _threads)
            t.join();
    }

    template<typename FunctionType>
    void addTask(FunctionType &&f){
        workQueue.push(std::forward<FunctionType>(f));
    }

    void stop(){
        _stop.store(true);
    }

private:
    void worker_thread(){
        try{
            while(!_stop && !_serviceStop){
                auto task = workQueue.checkPop();
                if(task){
                    (*task)();
                }
                else{
                    std::this_thread::yield();
                }
            }
        }
        catch(const std::exception &){
            // some critical event occurred, all service must be stopped
            _serviceStop=true;
            return;
        }
    }

private:
    const int _threadCount;
    std::atomic<bool> _stop{false};
    std::atomic<bool> &_serviceStop;
    ThreadSaveQueue<ThreadPoolFunc> workQueue;
    std::vector<std::thread> _threads;
};