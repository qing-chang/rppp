#pragma once

#include <sys/socket.h>
#include <sys/types.h>

#include "block_syscall.h"

class Socket;

class SocketConnectOperation : public BlockSyscall<SocketConnectOperation, int>
{
public:
    SocketConnectOperation(Socket* socket, void* buffer, std::size_t len);
    ~SocketConnectOperation();

    int syscall();
    void suspend();
private:
    Socket* socket;
    void* buffer_;
    std::size_t len_;
};

