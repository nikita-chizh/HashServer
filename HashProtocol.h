#pragma once
#include "CryptoFunctions.h"

class HashProtocol {
public:
    HashProtocol() = default;
    explicit HashProtocol(const std::string &hashFunction);

    void acceptClient(const int clientSock);
    ProcessRes processChunck(const int clientSock, const char* buf, const size_t size);
    void writeAnswer(const int clientSock, const char* data, size_t size);
    void closeClient(const int clientSock);
protected:
    void setCryptoFunc(CryptoFunc cryptoFunc);
    CryptoFunc _cryptoFunc;
private:
    void writeHashRes(const int clientSock, const std::string &hash);
private:
    std::unordered_map<int, std::vector<char>> _clients;
};


