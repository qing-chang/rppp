#pragma once

#include <sys/socket.h>
#include <sys/types.h>

// #include "block_syscall.h"
#include <cerrno>
#include <coroutine>
#include <type_traits>
#include <iostream>

template<typename SyscallOpt>
class BlockSyscall
{
public:
    BlockSyscall()
        : haveSuspend{false}
    {}

    bool await_ready() const noexcept { return false; }

    bool await_suspend(std::coroutine_handle<> h)
    {
        static_assert(std::is_base_of_v<BlockSyscall, SyscallOpt>);
        awaitingCoroutine = h;
        returnValue = static_cast<SyscallOpt*>(this)->syscall();
        haveSuspend =
            returnValue == -1 && (errno == EAGAIN || errno == EWOULDBLOCK);
        if (haveSuspend)
            static_cast<SyscallOpt*>(this)->suspend();

        return haveSuspend;
    }

    int await_resume()
    {
        std::cout << "await_resume\n";
        if (haveSuspend)
            returnValue = static_cast<SyscallOpt*>(this)->syscall();
        return returnValue;
    }

protected:
    bool haveSuspend;
    std::coroutine_handle<> awaitingCoroutine;
    int returnValue;
};
class Socket;

class SocketAcceptOperation : public BlockSyscall<SocketAcceptOperation>
{
public:
    SocketAcceptOperation(Socket* socket);
    ~SocketAcceptOperation();

    int syscall();
    void suspend();
private:
    Socket* socket;
    void* buffer_;
    std::size_t len_;
};