#include <iostream>
#include "socket_connect_operation.h"
#include "socket.h"

SocketConnectOperation::SocketConnectOperation(Socket* socket, std::string addr, std::size_t port)
    : socket{socket}
    , addr_{addr}
    , port_{port}
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
    struct sockaddr_in remote_addr;
    memset(&remote_addr,0,sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(port_);
    remote_addr.sin_addr.s_addr = inet_addr(addr_.c_str());
    std::cout << "connect(" << socket->fd << ", ...)\n";
    return connect(socket->fd, (struct sockaddr *)&remote_addr, sizeof(remote_addr));
}

void SocketConnectOperation::suspend()
{
    socket->coroSend_ = awaitingCoroutine;
}
int SocketConnectOperation::getsockopt_()
{
    int err;
    socklen_t socklen = sizeof(err);
    if(getsockopt(socket->fd, SOL_SOCKET, SO_ERROR, &err, &socklen) < 0)
        return -1;
    else if (err == 0)
        return 0;
    else
        return err;
}