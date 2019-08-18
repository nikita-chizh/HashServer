#pragma once
#include "Utils.h"


class HashProtocol {
public:
    HashProtocol() = default;
    void acceptClient(const int clientSock);
    ProcessRes processChunck(const int clientSock, const char* buf, const size_t size);
    void writeAnswer(const int clientSock, const char* data, size_t size);
    void closeClient(const int clientSock);
private:
    void writeHashRes(const int clientSock, const std::vector<char>& hash);
    std::vector<char> hash(const char* data, const size_t size);
private:
    std::unordered_map<int, std::vector<char>> _clients;
};


