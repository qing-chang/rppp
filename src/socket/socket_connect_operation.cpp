#include "socket_connect_operation.h"

#include <iostream>

#include "socket.h"

SocketConnectOperation::SocketConnectOperation(Socket* socket,
        void* buffer,
        std::size_t len)
    : socket{socket}
    , buffer_{buffer}
    , len_{len}
    , haveSuspend_{false}
{
    socket->io_context_.watchRead(socket);
    std::cout << "socket_connect_operation\n";
}

SocketConnectOperation::~SocketConnectOperation()
{
    socket->io_context_.unwatchRead(socket);
    std::cout << "~socket_connect_operation\n";
}

int SocketConnectOperation::connect_()
{
    std::cout << "connect(" << socket->fd << ", ...)\n";
    return connect(socket->fd, (struct sockaddr *)buffer_, len_);
}

void SocketConnectOperation::suspend()
{
    socket->coroRecv_ = awaitingCoroutine_;
}