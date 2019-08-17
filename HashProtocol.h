#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include "Utils.h"

class HashProtocol {
public:
    HashProtocol() = default;
    void acceptClient(int clientSock);
    ProcessRes processChunck(int clientSock, const char* buf, size_t size);
    void writeAnswer(int clientSock, const char* data, size_t size);
    void writeAnswer(int clientSock, const std::vector<char>& data);
    void closeClient(int clientSock);
private:
    void writeHashRes(int clientSock, const std::vector<char>& hash);
    std::vector<char> hash(const char* data, size_t size);
    std::vector<char> hash(const std::vector<char>& data);
private:
    std::unordered_map<int, std::vector<char>> _clients;
};


