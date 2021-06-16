#include "socket_connect_operation.h"

#include <iostream>

#include "socket.h"

SocketConnectOperation::SocketConnectOperation(Socket* socket,
        void* buffer,
        std::size_t len)
    : BlockSyscall2{}
    , socket{socket}
    , buffer_{buffer}
    , len_{len}
{
    socket->io_context_.watchRead(socket);
    std::cout << "socket_connect_operation\n";
}

SocketConnectOperation::~SocketConnectOperation()
{
    socket->io_context_.unwatchRead(socket);
    std::cout << "~socket_connect_operation\n";
}

int SocketConnectOperation::syscall()
{
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof their_addr;
    std::cout << "connect(" << socket->fd << ", ...)\n";
    return connect(socket->fd, (struct sockaddr *)buffer_, len_);
}

void SocketConnectOperation::suspend()
{
    socket->coroRecv_ = awaitingCoroutine_;
}