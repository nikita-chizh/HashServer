#include "HashProtocol.h"


void HashProtocol::acceptClient(const int clientSock){
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
        throw std::runtime_error("socket fd==" + std::to_string(clientSock) + " is not exist");

    auto endPos = std::find(buf, buf + size, '\n');
    if(endPos == buf + size){ // message end is not in this chunk
        auto curChunckEnd = targetUser->second.end();
        targetUser->second.insert(curChunckEnd, buf, buf + size);
        return {PROCESS_STATUS::NOT_FOUND, {}};
    }

    if(endPos < buf + size){// end of packet was found
        if(targetUser->second.empty()){ // if there is not any data from this user yet
            return {PROCESS_STATUS::FULL_IN_ONE, {}};
        }else{// end of chunk was accepted
            auto curChunckEnd = targetUser->second.end();
            targetUser->second.insert(curChunckEnd, buf, buf + size);
            return {PROCESS_STATUS::FOUND_IN_MANY, std::move(targetUser->second)};
        }
    }
    throw std::runtime_error("inconsistent processChunck scenario");
}

void HashProtocol::writeAnswer(const int clientSock, const char* data, size_t size){
    auto hashRes = hash(data, size);
    writeHashRes(clientSock, hashRes);
}

void HashProtocol::writeHashRes(const int clientSock, const std::vector<char>& hash){
    write(clientSock, hash.data(), hash.size());
}

std::vector<char> HashProtocol::hash(const char* data, const size_t size){
    std::vector<char> res;
    res.insert(res.end(), data, data + size);
    return res;
}


void HashProtocol::closeClient(const int clientSock){
    _clients.erase(clientSock);
}
