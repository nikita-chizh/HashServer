#include "HashProtocol.h"


HashProtocol::HashProtocol(const std::string &hashFunction){
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

void HashProtocol::acceptClient(const int clientSock){
    auto targetUser = _clients.find(clientSock);
    if(targetUser != _clients.end())
        throw std::runtime_error("ERROR acceptClient fd=" + std::to_string(clientSock) + " exists");

    std::vector<char> buf;
    buf.reserve(256);
    _clients.insert({clientSock, std::move(buf)});
}
/* There can be 3 variants:
 * 1. buf comprises full string to hash
 * 2. buf with already accumulated data make up a full string
 * 3. buf doesnt have \n so we need to store it's data
 * */

ProcessRes HashProtocol::processChunck(const int clientSock, const char* buf, const size_t size){
    auto targetUser = _clients.find(clientSock);
    if(targetUser == _clients.end())
        throw std::runtime_error("ERROR processChunck fd=" + std::to_string(clientSock) + " doesn't exist");

    auto endPos = std::find(buf, buf + size, '\n');
    if(endPos == buf + size){ // message end is not in this chunk
        auto curChunckEnd = targetUser->second.end();
        targetUser->second.insert(curChunckEnd, buf, buf + size);
        return ProcessRes{PROCESS_STATUS::NOT_FOUND, {}};
    }

    if(endPos < buf + size){// end of packet was found
        if(targetUser->second.empty()){ // if there is not any data from this user yet
            return ProcessRes{PROCESS_STATUS::FULL_IN_ONE, {}};
        }else{// end of chunk was accepted
            auto curChunckEnd = targetUser->second.end();
            targetUser->second.insert(curChunckEnd, buf, buf + size);
            return ProcessRes{PROCESS_STATUS::FOUND_IN_MANY, std::move(targetUser->second)};
        }
    }
    throw std::runtime_error("ERROR inconsistent processChunck scenario");
}

void HashProtocol::writeAnswer(const int clientSock, const char* data, size_t size){
    auto targetUser = _clients.find(clientSock);
    if(targetUser == _clients.end())
        throw std::runtime_error("ERROR writeAnswer socket fd=" + std::to_string(clientSock) + " is not exist");

    auto hashRes = _cryptoFunc(data, size-1);// without \n
    writeHashRes(clientSock, hashRes);
    closeClient(clientSock);
}

void HashProtocol::writeHashRes(const int clientSock, const std::string &hash){
    auto err = write(clientSock, hash.data(), hash.size());
    logIf(lessThenZero, err, "ERROR writeHashRes socket fd=" + std::to_string(clientSock));
}

void HashProtocol::closeClient(const int clientSock){
    auto err = close(clientSock);
    logIf(lessThenZero, err, "ERROR on close on closeClient");
    _clients.erase(clientSock);

}
