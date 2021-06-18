#include "socket.h"

#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>

Socket::Socket(IOContext& io_context, int port)
    : io_context_{io_context}
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
    if(port != 0)
    {
        struct sockaddr_in local_addr;
        memset(&local_addr,0,sizeof(local_addr));
        local_addr.sin_family = AF_INET;
        local_addr.sin_port = htons(port);
        local_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
        if (bind(fd, (struct sockaddr*)&local_addr, sizeof(local_addr)) == -1)
            throw std::runtime_error{"bind"};
        listen(fd, 8);
    }
    fcntl(fd, F_SETFL, O_NONBLOCK);
    io_context_.attach(this);
    io_context_.watchRead(this);
}

Socket::Socket(Socket&& socket)
    : io_context_{socket.io_context_}
    , fd{socket.fd}
    , io_state_{socket.io_state_}
    , io_new_state_{socket.io_new_state_}
{
    socket.fd = -1;
}

Socket::Socket(int fd, IOContext& io_context)
    : io_context_{io_context}
    , fd{fd}
{
    fcntl(fd, F_SETFL, O_NONBLOCK);
    io_context_.attach(this);
}

Socket::~Socket()
{
    if (fd == -1)
        return;
    io_context_.detach(this);
    std::cout << "close(" << fd << ")\n";
    close(fd);
}

std::task<std::shared_ptr<Socket>> Socket::accept()
{
    int fd = co_await SocketAcceptOperation{this};
    if (fd == -1)
        throw std::runtime_error{"accept"};
    co_return std::shared_ptr<Socket>(new Socket{fd, io_context_});
}

std::task<int> Socket::connect(std::string addr, std::size_t port)
{
    struct sockaddr_in remote_addr;
    memset(&remote_addr,0,sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(port);
    remote_addr.sin_addr.s_addr = inet_addr(addr.c_str());
    int c = co_await SocketConnectOperation{this,(void *)(&remote_addr),sizeof(remote_addr)};
    std::cout << "连接。。。。。"<<c<<"\n";
    // if (c == -1)
    //     throw std::runtime_error{"connect"};
    co_return c;
}

SocketRecvOperation Socket::recv(void* buffer, std::size_t len)
{
    return SocketRecvOperation{this, buffer, len};
}

SocketSendOperation Socket::send(void* buffer, std::size_t len)
{
    return SocketSendOperation{this, buffer, len};
}