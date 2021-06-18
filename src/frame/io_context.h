#pragma once

#include <set>
#include <stdexcept>
#include <sys/epoll.h>

#include "time.h"
#include "../socket/socket_accept_operation.h"
#include "../socket/socket_connect_operation.h"
#include "../socket/socket_recv_operation.h"
#include "../socket/socket_send_operation.h"

/* Just an epoll wrapper */
class Socket;

class IOContext
{
public:
    IOContext(){}
    void init();
    void expireTimer();
    void run();
private:
    constexpr static std::size_t max_events = 10;
    int efd;

    // Fill it by watchRead / watchWrite
    std::set<Socket*> processedSockets;

    friend Socket;
    friend SocketAcceptOperation;
    friend SocketConnectOperation;
    friend SocketRecvOperation;
    friend SocketSendOperation;
    void attach(Socket* socket);
    void watchConnect(Socket* socket);
    void watchRead(Socket* socket);
    void unwatchRead(Socket* socket);
    void watchWrite(Socket* socket);
    void unwatchWrite(Socket* socket);
public:
    void detach(Socket* socket);
};
