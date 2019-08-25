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
/* There can be 3 variants:
 * 1. testMsg comprises full string to hash
 * 2. testMsg with already accumulated data make up a full string
 * 3. testMsg doesnt have \n so we need to store it's data
 * */

ProcessRes HashProtocol::processChunk(const int &clientSock, const char* buf, const size_t &size){
    auto targetUser = _clients.find(clientSock);
    if(targetUser == _clients.end())
        throw std::runtime_error("ERROR processChunk fd=" + std::to_string(clientSock) + " doesn't exist");
    auto endPos = std::find(buf, buf + size, '\n');
    if(endPos == buf + size){ // message end is not in this chunk
        auto curChunkEnd = targetUser->second.end();
        targetUser->second.insert(curChunkEnd, buf, buf + size);
        return ProcessRes{PROCESS_STATUS::NOT_FOUND, {}};
    }

    if(endPos < buf + size){// end of packet was found
        auto res = ProcessRes{PROCESS_STATUS::FULL_IN_ONE, {}}; // default case, when fuul msg is in buf

        if(!targetUser->second.empty()){ // if there there was a data from this user
            auto curChunkEnd = targetUser->second.end();
            targetUser->second.insert(curChunkEnd, buf, buf + size);
            res = ProcessRes{PROCESS_STATUS::FOUND_IN_MANY, std::move(targetUser->second)};
        }
        _clients.erase(clientSock);
        return res;
    }
    throw std::runtime_error("ERROR inconsistent processChunk scenario");
}

void HashProtocol::writeAnswer(const int &clientSock, const char* data, const size_t &size){
    auto hashRes = _cryptoFunc(data, size-1);// without \n
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
