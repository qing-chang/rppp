#pragma once

#include <sys/socket.h>
#include <sys/types.h>

// #include "block_syscall.h"
#include <cerrno>
#include <coroutine>
#include <type_traits>
#include <iostream>

// template<typename SyscallOpt>
// class BlockSyscall2
// {
// public:
//     BlockSyscall2()
//         : haveSuspend_{false}
//     {}


// };

class Socket;

class SocketConnectOperation// : public BlockSyscall2<SocketConnectOperation>
{
public:
    SocketConnectOperation(Socket* socket, void* buffer, std::size_t len);
    ~SocketConnectOperation();

    int syscall();
    void suspend();

    bool await_ready() const noexcept { return false; }

    bool await_suspend(std::coroutine_handle<> h)
    {
        // static_assert(std::is_base_of_v<BlockSyscall2, SyscallOpt>);
        awaitingCoroutine_ = h;
        returnValue_ = syscall();
        haveSuspend_ =
            returnValue_ == -1 && (errno == EAGAIN || errno == EWOULDBLOCK);
        if (haveSuspend_)
            suspend();

        return haveSuspend_;
    }

    int await_resume()
    {
        std::cout << "await_resume\n";
        if (haveSuspend_)
            returnValue_ = syscall();
        return returnValue_;
    }

protected:
    bool haveSuspend_;
    std::coroutine_handle<> awaitingCoroutine_;
    int returnValue_;
private:
    Socket* socket;
    void* buffer_;
    std::size_t len_;
};