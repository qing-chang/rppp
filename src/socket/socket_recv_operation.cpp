#include "socket_recv_operation.h"
#include <iostream>
#include "socket.h"

SocketRecvOperation::SocketRecvOperation(Socket* socket, void* buffer, std::size_t len)
    : socket{socket}
    , buffer_{buffer}
    , len_{len}
    , haveSuspend{false}
{
    socket->io_context_.watchRead(socket);
    std::cout << "socket_recv_operation\n";
}

SocketRecvOperation::~SocketRecvOperation()
{
    socket->io_context_.unwatchRead(socket);
    std::cout << "~socket_recv_operation\n";
}

ssize_t SocketRecvOperation::syscall()
{
    std::cout << "recv(" << socket->fd << ", buffer_, len_, 0)\n";
    return recv(socket->fd, buffer_, len_, 0);
}

void SocketRecvOperation::suspend()
{
    socket->coroRecv_ = awaitingCoroutine;
}