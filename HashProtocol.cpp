#include "HashProtocol.h"


void HashProtocol::acceptClient(int clientSock){
    std::vector<char> buf;
    buf.reserve(256);
    _clients.insert({clientSock, std::move(buf)});
}

ProcessRes HashProtocol::processChunck(int clientSock, const char* buf, size_t size){
    auto targetUser = _clients.find(clientSock);
    if(targetUser == _clients.end())
    {
        throw std::runtime_error("socket fd==" + std::to_string(clientSock) + " is not exist");
    }
    auto endPos = std::find(buf, buf + size, '\n');

    if(endPos == buf + size){ // message end is not in this chunk
        auto curChunckEnd = targetUser->second.end();
        targetUser->second.insert(curChunckEnd, buf, buf + size);
        return {false, ProcessData{nullptr}};
    }

    if(endPos < buf + size){// end of packet was found
        if(targetUser->second.empty()){ // if there is not any data from this user yet
            return {true, ProcessData{buf}};
        }else{// full string was accepted
            auto curChunckEnd = targetUser->second.end();
            targetUser->second.insert(curChunckEnd, buf, buf + size);
            return {true, ProcessData{std::move(targetUser->second)}};
        }
    }
    throw std::runtime_error("inconsistent processChunck scenario");
}

void HashProtocol::writeAnswer(int clientSock, const char* data, size_t size){
    auto hashRes = hash(data, size);
    writeHashRes(clientSock, hashRes);
}

void HashProtocol::writeAnswer(int clientSock, const std::vector<char>& data){
    auto hashRes = hash(data);
    writeHashRes(clientSock, hashRes);
}

void writeHashRes(int clientSock, const std::vector<char>& hash){
    //write(clientSock, hash.data(), hash.size());
}



void HashProtocol::closeClient(int clientSock){
    _clients.erase(clientSock);
}
