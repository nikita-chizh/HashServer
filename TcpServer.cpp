#include <iostream>
#include "TcpServer.h"
const size_t CLIENT_BUF_SIZE = 1024;

TcpServer::TcpServer(uint16_t port):loop(0){
    _srvAddr = {};
    _srvAddr.sin_family = AF_INET;
    _srvAddr.sin_port = htons(port);
    _srvAddr.sin_addr.s_addr = INADDR_ANY;
    //
    _serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    //
    throwIf(lessThenZero, _serverSocket, "ERROR: cannot create server Socket");
}

void TcpServer::bindSocket(){
    auto err = bind(_serverSocket, reinterpret_cast<sockaddr *>(&_srvAddr), sizeof(_srvAddr));
    throwIf(notEqZero, err, "ERROR: cannot bindSocket server Socket");
}

void TcpServer::startListen(){
    auto err = listen(_serverSocket, _clientBacklog);
    throwIf(notEqZero, err, "ERROR: cannot listen server Socket");
}

void TcpServer::startServer(){
    _acceptIO.data = reinterpret_cast<void*>(this);
    ev_io_init(&_acceptIO, acceptConnection, _serverSocket, EV_READ);
    ev_io_start(loop.loop, &_acceptIO);
    while (!_stop)
    {
        ev_loop(loop.loop, 0);
    }

}

void TcpServer::acceptConnection(struct ev_loop *loop, struct ev_io *acceptIO, int revents){
    if(EV_ERROR & revents)
    {
        std::cerr << "error in acceptConnection"<<std::endl;
        return;
    }

    auto *server = reinterpret_cast<TcpServer*>(acceptIO->data);
    sockaddr_in client_addr = {};
    socklen_t client_len = sizeof(client_addr);
    auto *clientIO = new ev_io;
    clientIO -> data = reinterpret_cast<void*>(server);

    int clientSocket = accept(acceptIO->fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
    if (clientSocket < 0)
    {
        std::cerr << "error in accept socket"<<std::endl;
        return;
    }
    server -> _acceptLogic(clientSocket);

    ev_io_init(clientIO, readData, clientSocket, EV_READ);
    ev_io_start(loop, clientIO);
}

void TcpServer::readData(struct ev_loop *loop, struct ev_io *clientIO, int revents){
    if(EV_ERROR & revents)
    {
        std::cerr << "error in readData"<<std::endl;
        return;
    }

    char buffer[CLIENT_BUF_SIZE];
    auto *server = reinterpret_cast<TcpServer*>(clientIO->data);

    ssize_t readBytes = recv(clientIO->fd, buffer, CLIENT_BUF_SIZE, 0);

    if(readBytes < 0)
    {
        std::cerr << "recv retyrned with error"<<std::endl;
        return;
    }
    // client has closed connection
    if(readBytes == 0){
        ev_io_stop(loop, clientIO);
        free(clientIO);
        return;
    }else{
        // protocol handler
        server->_processLogic(clientIO->fd, buffer, readBytes);
    }
    memset(buffer, 0, readBytes);
}




