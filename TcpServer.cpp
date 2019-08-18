#include "TcpServer.h"
#include <iostream>
const size_t CLIENT_BUF_SIZE = 1024;

TcpServer::TcpServer(const uint16_t port, const ServerLogic &logic):_port(port),_loop(0),_logic(logic) {

}

TcpServer::TcpServer(TcpServer &&server):_port(server._port),_loop(0),_logic(server._logic){

}

void TcpServer::bindSocket(){
    _srvAddr = {};
    _srvAddr.sin_family = AF_INET;
    _srvAddr.sin_port = htons(_port);
    _srvAddr.sin_addr.s_addr = INADDR_ANY;
    //
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    throwIf(lessThenZero, _serverSocket, "ERROR: cannot create server Socket");
    //
    int help = 1;
    auto err = setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEPORT, &help, sizeof(help));
    throwIf(lessThenZero, err, "ERROR setsockopt");
    //
    err = bind(_serverSocket, reinterpret_cast<sockaddr *>(&_srvAddr), sizeof(_srvAddr));
    throwIf(notEqZero, err, "ERROR: cannot bindSocket server Socket");
}

void TcpServer::startListen(){
    auto err = listen(_serverSocket, _clientBacklog);
    throwIf(notEqZero, err, "ERROR: cannot listen server Socket");
}

void TcpServer::initLoop(){
    _acceptIO.data = reinterpret_cast<void*>(this);
    ev_io_init(&_acceptIO, acceptConnection, _serverSocket, EV_READ);
    ev_io_start(_loop.loop, &_acceptIO);
}

void TcpServer::process(){
    ev_loop(_loop.loop, 0);
}

void TcpServer::stop(){
    ev_io_stop(_loop.loop, &_acceptIO);
}

void TcpServer::acceptConnection(struct ev_loop *loop, struct ev_io *acceptIO, int revents){
    if(EV_ERROR & revents)
    {
        std::cerr << "error in acceptConnection"<<std::endl;
        return;
    }
    ev_io *clientIO = nullptr;
    try {
        auto *server = reinterpret_cast<TcpServer *>(acceptIO->data);
        sockaddr_in client_addr = {};
        socklen_t client_len = sizeof(client_addr);
        clientIO = new ev_io;
        clientIO->data = reinterpret_cast<void *>(server);

        int clientSocket = accept(acceptIO->fd, reinterpret_cast<sockaddr *>(&client_addr), &client_len);
        if (clientSocket < 0) {
            std::cerr << "error in accept socket" << std::endl;
            return;
        }
        server->_logic.accept(clientSocket);

        ev_io_init(clientIO, readData, clientSocket, EV_READ);
        ev_io_start(loop, clientIO);
    }
    catch (const std::exception &error){
        std::cerr<<error.what();
        ev_io_stop(loop, clientIO);
        delete clientIO;
        throw;
    }
}

void TcpServer::readData(struct ev_loop *loop, struct ev_io *clientIO, int revents){
    if(EV_ERROR & revents)
    {
        std::cerr << "error in readData"<<std::endl;
        return;
    }
    try {
        char buffer[CLIENT_BUF_SIZE];
        auto *server = reinterpret_cast<TcpServer *>(clientIO->data);

        const auto clientSocket = clientIO->fd;
        ssize_t readBytes = recv(clientSocket, buffer, CLIENT_BUF_SIZE, 0);

        if (readBytes < 0) {
            std::cerr << "recv retyrned with error" << std::endl;
            return;
        }
        // client has closed connection
        if (readBytes == 0) {
            server->_logic.close(clientSocket);
            ev_io_stop(loop, clientIO);
            delete clientIO;
            return;
        } else {
            // protocol handler
            auto[status, processedData] = server->_logic.process(clientSocket, buffer, readBytes);
            if (status == PROCESS_STATUS::FULL_IN_ONE)
                server->_logic.answer(clientSocket, buffer, readBytes);
            if (status == PROCESS_STATUS::FOUND_IN_MANY)
                server->_logic.answer(clientSocket, processedData.data(), processedData.size());
            if(status != PROCESS_STATUS::NOT_FOUND) {// answer was send and socket closed
                ev_io_stop(loop, clientIO);
                delete clientIO;
                return;
            }
        }
    }
    catch (const std::exception &error){
        std::cerr<<error.what();
        throw;
    }
}





