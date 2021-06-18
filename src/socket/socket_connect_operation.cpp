#include <iostream>
#include "socket_connect_operation.h"
#include "socket.h"

SocketConnectOperation::SocketConnectOperation(Socket* socket, void* addr, std::size_t len)
    : socket{socket}
    , addr_{addr}
    , len_{len}
    , haveSuspend{false}
{
    socket->io_context_.watchConnect(socket);
    std::cout << "socket_connect_operation\n";
}

SocketConnectOperation::~SocketConnectOperation()
{
    socket->io_context_.unwatchWrite(socket);
    std::cout << "~socket_connect_operation\n";
}

int SocketConnectOperation::connect_()
{
    std::cout << "connect(" << socket->fd << ", ...)\n";
    return connect(socket->fd, (struct sockaddr *)addr_, len_);
}

void SocketConnectOperation::suspend()
{
    socket->coroSend_ = awaitingCoroutine;
}