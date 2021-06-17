#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <cerrno>
#include <coroutine>
#include <type_traits>
#include <iostream>

class Socket;

class SocketRecvOperation
{
public:
    SocketRecvOperation(Socket* socket, void* buffer, std::size_t len);
    ~SocketRecvOperation();

    ssize_t syscall();
    void suspend();

    bool await_ready() const noexcept { return false; }

    bool await_suspend(std::coroutine_handle<> h)
    {
        awaitingCoroutine = h;
        returnValue = syscall();
        haveSuspend =
            returnValue == -1 && (errno == EAGAIN || errno == EWOULDBLOCK);
        if (haveSuspend)
            suspend();
        return haveSuspend;
    }

    ssize_t await_resume()
    {
        std::cout << "await_resume\n";
        if (haveSuspend)
            returnValue = syscall();
        return returnValue;
    }

private:
    bool haveSuspend;
    std::coroutine_handle<> awaitingCoroutine;
    ssize_t returnValue;
    Socket* socket;
    void* buffer_;
    std::size_t len_;
};