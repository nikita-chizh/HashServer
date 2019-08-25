#include "HashProtocol.h"
const size_t DEFAULT_BUFFER_SIZE = 256;

HashProtocol::HashProtocol(const std::string &hashFunction, ThreadPool &threadPoll):_threadPoll(threadPoll){
    if(hashFunction == "sha256")
        setCryptoFunc(SHA256);
    else if(hashFunction == "sha512")
        setCryptoFunc(SHA512);
    else
        throw std::runtime_error("ERROR Function: " + hashFunction + "is not supported");
}

void HashProtocol::setCryptoFunc(CryptoFunc cryptoFunc){
    _cryptoFunc = cryptoFunc;
}

void HashProtocol::acceptClient(const int &clientSock){
    auto targetUser = _clients.find(clientSock);
    if(targetUser != _clients.end())
        throw std::runtime_error("ERROR acceptClient fd=" + std::to_string(clientSock) + " exists");
    std::vector<char> buf;
    buf.reserve(DEFAULT_BUFFER_SIZE);
    _clients.insert({clientSock, std::move(buf)});
}

std::vector<char> HashProtocol::processChunk(const int &clientSock, const char* buf, const size_t &size){
    auto targetUser = _clients.find(clientSock);
    if(targetUser == _clients.end())
        throw std::runtime_error("ERROR processChunk fd=" + std::to_string(clientSock) + " doesn't exist");

    std::vector<char> res = {};
    for(size_t i = 0; i < size; ++i){
        if(buf[i] == '\n'){// result chunk without \n
            res = std::move(targetUser->second);
            _clients.erase(clientSock);
            return res;
        }
        targetUser->second.push_back(buf[i]);
    }
    return res;
}

void HashProtocol::writeAnswer(const int &clientSock, const char* data, const size_t &size){
    std::string hashRes = {};
    if(size == 0)
        hashRes = "\n";
    else
        hashRes = _cryptoFunc(data, size);
    writeHashRes(clientSock, hashRes);
    sockClose(clientSock);
}

void HashProtocol::writeAsyncAnswer(const int &clientSock, std::vector<char> &&data){
    auto task = [this, clientSock, data = std::move(data)](){
        writeAnswer(clientSock, data.data(), data.size());
    };
    _threadPoll.addTask(std::move(task));
}

void HashProtocol::writeHashRes(const int &clientSock, const std::string &hash){
    auto err = write(clientSock, hash.data(), hash.size());
    logIf(lessThenZero, err, "ERROR writeHashRes socket fd=" + std::to_string(clientSock));
}

void HashProtocol::closeClient(const int &clientSock){
    _clients.erase(clientSock);
    sockClose(clientSock);
}

void HashProtocol::sockClose(const int &clientSock){
    auto err = close(clientSock);
    logIf(lessThenZero, err, "ERROR sockClose socket fd="+ std::to_string(clientSock));
}
