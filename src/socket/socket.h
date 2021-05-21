#pragma once

#include <cstring>
#include <memory>
#include <optional>

#include "../frame/io_context.h"
#include "socket_accept_operation.h"
#include "socket_connect_operation.h"
#include "socket_recv_operation.h"
#include "socket_send_operation.h"
#include "../frame/task.h"

class Socket
{
public:
    /* Listen tcp non blocking socket */
    Socket(IOContext& io_context, int port);
    Socket(const Socket&) = delete;
    Socket(Socket&& socket);

    ~Socket();

    std::task<std::shared_ptr<Socket>> accept();
    std::task<int> connect(std::string addr, std::size_t port);
    SocketRecvOperation recv(void* buffer, std::size_t len);
    SocketSendOperation send(void* buffer, std::size_t len);

    bool resumeRecv()
    {
        if (!coroRecv_)
            return false;
        coroRecv_.resume();
        return true;
    }

    bool resumeSend()
    {
        if (!coroSend_)
            return false;
        coroSend_.resume();
        return true;
    }

private:
    friend SocketAcceptOperation;
    friend SocketConnectOperation;
    friend SocketRecvOperation;
    friend SocketSendOperation;
    IOContext& io_context_;
    int fd = -1;
    friend IOContext;
    uint32_t io_state_ = 0;
    uint32_t io_new_state_ = 0;

    explicit Socket(int fd, IOContext& io_context);
    std::coroutine_handle<> coroRecv_;
    std::coroutine_handle<> coroSend_;
};
