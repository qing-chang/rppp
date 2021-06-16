#pragma once

#include <sys/socket.h>
#include <sys/types.h>

// #include "block_syscall.h"
#include <cerrno>
#include <coroutine>
#include <type_traits>
#include <iostream>

template<typename SyscallOpt, typename ReturnValue>
class BlockSyscall2
{
public:
    BlockSyscall2()
        : haveSuspend_{false}
    {}

    bool await_ready() const noexcept { return false; }

    bool await_suspend(std::coroutine_handle<> h)
    {
        // static_assert(std::is_base_of_v<BlockSyscall2, SyscallOpt>);
        awaitingCoroutine_ = h;
        returnValue_ = static_cast<SyscallOpt*>(this)->syscall();
        haveSuspend_ =
            returnValue_ == -1 && (errno == EAGAIN || errno == EWOULDBLOCK);
        if (haveSuspend_)
            static_cast<SyscallOpt*>(this)->suspend();

        return haveSuspend_;
    }

    ReturnValue await_resume()
    {
        std::cout << "await_resume\n";
        if (haveSuspend_)
            returnValue_ = static_cast<SyscallOpt*>(this)->syscall();
        return returnValue_;
    }

protected:
    bool haveSuspend_;
    std::coroutine_handle<> awaitingCoroutine_;
    ReturnValue returnValue_;
};

class Socket;

class SocketConnectOperation : public BlockSyscall2<SocketConnectOperation, int>
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