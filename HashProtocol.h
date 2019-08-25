#pragma once
#include "CryptoFunctions.h"
#include "ThreadPool.h"

class HashProtocol {
public:
    explicit HashProtocol(const std::string &hashFunction, ThreadPool &threadPoll);

    void acceptClient(const int &clientSock);
    ProcessRes processChunck(const int &clientSock, const char* buf, const size_t &size);
    void writeAsyncAnswer(const int &clientSock, std::vector<char> &&data);
    void closeClient(const int &clientSock);
protected:
    void setCryptoFunc(CryptoFunc cryptoFunc);
    CryptoFunc _cryptoFunc;
private:
    void writeAnswer(const int &clientSock, const char* data, const size_t &size);
    static void writeHashRes(const int &clientSock, const std::string &hash);
    static void sockClose(const int &clientSock);
private:
    std::unordered_map<int, std::vector<char>> _clients;
    ThreadPool &_threadPoll;
};


