#pragma once

#include <sys/socket.h>
#include <sys/types.h>

#include "block_syscall.h"

class Socket;

class SocketRecvOperation : public BlockSyscall<SocketRecvOperation, ssize_t>
{
public:
    SocketRecvOperation(Socket* socket, void* buffer, std::size_t len);
    ~SocketRecvOperation();

    ssize_t syscall();
    void suspend();
private:
    Socket* socket;
    void* buffer_;
    std::size_t len_;
};