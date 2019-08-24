#pragma once

#include <string>

struct Config {
    Config() = delete;
    explicit Config(const std::string &pathToConfig);
    explicit Config(std::string &&pathToConfig);
    void init(const std::string &pathToConfig);
    std::string hashFunction;
    uint16_t port;
    uint16_t numberOfAcceptors;
    uint16_t numberOfProcessors;
};
