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
    SocketConnectOperation(Socket* socket, void* addr, std::size_t len);
    ~SocketConnectOperation();

    int connect_();
    void suspend();

    bool await_ready() const noexcept { return false; }

    bool await_suspend(std::coroutine_handle<> h)
    {
        awaitingCoroutine = h;
        int res = connect_();
        if (res != -1)
        {
            std::cout << "直接连接成功\n";
            returnValue = 0;
        }
        else if(errno == EINPROGRESS)
        {
            std::cout << "正在连接中\n";
            haveSuspend = true;
            suspend();
        }
        else
        {
            std::cout << "连接失败\n";
            returnValue = -1;
        }
        return haveSuspend;//false;
    }

    int await_resume()
    {
        std::cout << "连接await_resume\n"<<returnValue<<"\n";
        if(haveSuspend)
        {
            returnValue = 0;
        }
        return returnValue;
    }

private:
    bool haveSuspend;
    std::coroutine_handle<> awaitingCoroutine;
    int returnValue;
    Socket* socket;
    void* addr_;
    std::size_t len_;
};