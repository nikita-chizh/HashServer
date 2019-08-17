#include "TcpServer.h"

TcpServer::TcpServer(uint16_t port):loop(0){
    _srvAddr = {};
    _srvAddr.sin_family = AF_INET;
    _srvAddr.sin_port = htons(port);
    _srvAddr.sin_addr.s_addr = INADDR_ANY;
    //
    _serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    //
    auto errorText = std::string("ERROR: cannot create server Socket");
    throwIf(lessThenZero, _serverSocket, errorText);
}

void TcpServer::bindSocket(){
    auto err = bind(_serverSocket, reinterpret_cast<sockaddr *>(&_srvAddr), sizeof(_srvAddr));
    //throwIf(notEqZero, err, "ERROR: cannot bindSocket server Socket");
}

void TcpServer::startListen(){
    auto err = listen(_serverSocket, _clientBacklog);
    //throwIf(notEqZero, err, "ERROR: cannot listen server Socket");
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

void TcpServer::acceptConnection(struct ev_loop *loop, struct ev_io *watcher, int revents){
    auto *server = reinterpret_cast<TcpServer*>(watcher->data);

    sockaddr_in client_addr = {};
    socklen_t client_len = sizeof(client_addr);
    ev_io *w_client = (struct ev_io*) malloc (sizeof(struct ev_io));

    if(EV_ERROR & revents)
    {
        perror("got invalid event");
        return;
    }
    int clientSocket = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);

    if (clientSocket < 0)
    {
        perror("accept error");
        return;
    }
    ev_io_init(w_client, readData, clientSocket, EV_READ);
    ev_io_start(loop, w_client);
}

void TcpServer::readData(struct ev_loop *loop, struct ev_io *watcher, int revents){
    char buffer[BUFFER_SIZE];

    if(EV_ERROR & revents)
        return;

    ssize_t readBytes = recv(watcher->fd, buffer, BUFFER_SIZE, 0);

    if(readBytes < 0)
        return;

    if(readBytes == 0){
        ev_io_stop(loop,watcher);
        free(watcher);
        return;
    }else{
        printf("message:%s\n",buffer);
    }
    send(watcher->fd, buffer, readBytes, 0);
    bzero(buffer, readBytes);
}




