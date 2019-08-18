#pragma once

#include <string>

struct Config {
    Config() = delete;
    Config(const std::string &pathToConfig);
    Config(std::string &&pathToConfig);
    void init(const std::string &pathToConfig);
    std::string hashFunction;
    uint16_t port;
    uint16_t numberOfAcceptors;
};
