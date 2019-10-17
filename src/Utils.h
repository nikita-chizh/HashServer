#pragma once
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
//
#include <iostream>
#include <functional>
#include <string>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include <vector>
//
#include <ev.h>

inline bool lessThenZero(int value){
    return value < 0;
}

inline bool notEqZero(int value){
    return value != 0;
}

inline void throwIf(  std::function<bool(int)> pridicate
        , const int value
        , std::string& errorText)
{
    if(pridicate(value))
        throw std::runtime_error(errorText + ". errno: " + strerror(errno));

}

inline void throwIf(  std::function<bool(int)> pridicate
                    , const int value
                    , std::string&& errorText)
{
    throwIf(pridicate, value, errorText);
}

inline void logIf(  std::function<bool(int)> pridicate
        , const int value
        , std::string& errorText)
{
    if(pridicate(value))
        std::cerr << errorText + ". errno: " + strerror(errno);

}

inline void logIf(  std::function<bool(int)> pridicate
        , const int value
        , std::string&& errorText)
{
    logIf(pridicate, value, errorText);
}


struct SimpleLoop
{
    SimpleLoop(unsigned int flags = 0);
    ~SimpleLoop();
    struct ev_loop *loop;
};
