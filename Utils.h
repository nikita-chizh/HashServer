#pragma once
#include <netinet/in.h>
#include <errno.h>
//
#include <functional>
#include <variant>
#include <string>
//
#include <ev.h>

using ProcessData = std::variant<const char*, std::vector<char>>;
using ProcessRes = std::pair<bool, ProcessData>;

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

struct SimpleLoop
{
    SimpleLoop(unsigned int flags = 0);
    ~SimpleLoop();
    struct ev_loop *loop;
};
