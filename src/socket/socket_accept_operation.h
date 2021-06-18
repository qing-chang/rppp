#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <cerrno>
#include <coroutine>
#include <type_traits>
#include <iostream>

class Socket;

class SocketAcceptOperation
{
public:
    SocketAcceptOperation(Socket* socket);
    ~SocketAcceptOperation();

    int accept_();
    void suspend();
    bool await_ready() const noexcept { return false; }

    bool await_suspend(std::coroutine_handle<> h)
    {
        awaitingCoroutine = h;
        returnValue = accept_();
        haveSuspend =
            returnValue == -1 && (errno == EAGAIN || errno == EWOULDBLOCK);
        if (haveSuspend)
            suspend();
        return haveSuspend;
    }

    int await_resume()
    {
        std::cout << "await_resume\n";
        if (haveSuspend)
            returnValue = accept_();
        return returnValue;
    }

private:
    bool haveSuspend;
    std::coroutine_handle<> awaitingCoroutine;
    int returnValue;
    Socket* socket;
    void* buffer_;
    std::size_t len_;
};