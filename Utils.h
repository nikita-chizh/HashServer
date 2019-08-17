#pragma once
#include <functional>
#include <string>
#include <errno.h>
//
#include <ev.h>

inline bool lessThenZero(int value){
    return value < 0;
}

inline bool notEqZero(int value){
    return value != 0;
}

template <typename T>
inline void throwIf(std::function<bool(int)> pridicate, const int value, T&& errorText)
{
    if(pridicate(value))
        throw std::runtime_error(errorText + ". errno: " + strerror(errno));
}

struct SimpleLoop
{
    SimpleLoop(unsigned int flags = 0);
    ~SimpleLoop();
    struct ev_loop *loop;
};
