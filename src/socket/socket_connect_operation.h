#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <cerrno>
#include <coroutine>
#include <type_traits>
#include <iostream>

class Socket;

class SocketConnectOperation
{
public:
    SocketConnectOperation(Socket* socket, void* buffer, std::size_t len);
    ~SocketConnectOperation();

    int connect_();
    void suspend();

    bool await_ready() const noexcept { return false; }

    bool await_suspend(std::coroutine_handle<> h)
    {
        awaitingCoroutine_ = h;
        returnValue_ = connect_();
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
            returnValue_ = connect_();
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