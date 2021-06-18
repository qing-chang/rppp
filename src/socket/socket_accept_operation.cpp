#include "socket_accept_operation.h"

#include <iostream>

#include "socket.h"

SocketAcceptOperation::SocketAcceptOperation(Socket* socket)
    : BlockSyscall{}
    , socket{socket}
{
    socket->io_context_.watchRead(socket);
    std::cout << "socket_accept_operation\n";
}

SocketAcceptOperation::~SocketAcceptOperation()
{
    socket->io_context_.unwatchRead(socket);
    std::cout << "~socket_accept_operation\n";
}

int SocketAcceptOperation::syscall()
{
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof their_addr;
    std::cout << "accept(" << socket->fd << ", ...)\n";
    return accept(socket->fd, (struct sockaddr *) &their_addr, &addr_size);
}

void SocketAcceptOperation::suspend()
{
    socket->coroRecv_ = awaitingCoroutine;
}