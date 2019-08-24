#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

template<typename T>
class ThreadSaveQueue
{
public:
    ThreadSaveQueue() = default;

    template <class U>
    void push(U &&new_value){
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(std::forward<U>(new_value));
        data_cond.notify_one();
    }

    std::unique_ptr<T> waitPop(){
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        auto res = std::make_unique<T>(std::move(data_queue.front()));
        data_queue.pop();
        return res;
    }

    std::unique_ptr<T> checkPop(){
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return nullptr;
        auto res = std::make_unique<T>(std::move(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool size() const{
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.size();
    }

private:
    std::queue<T> data_queue;
    mutable std::mutex mut;
    std::condition_variable data_cond;

};
