#include "io_context.h"
#include <stdexcept>
#include "../socket/socket.h"

timerNode *timers;

void IOContext::init()
{
    efd = epoll_create1(0);
    if (efd == -1)
        throw std::runtime_error{"epoll_create1"};
}

void IOContext::expireTimer()
{
    timerNode *ctn = timers;
    while(ctn && clock() > ctn->time)
    {
        ctn->h.resume();
        ctn = ctn->next;
    }
}

void IOContext::run()
{
    struct epoll_event events[max_events];
    for (;;)
    {
        expireTimer();
        clock_t tv= timers?timers->time - clock():-1;
        auto nfds = epoll_wait(efd, events, max_events, tv);
        expireTimer();
        if (nfds == -1)
            throw std::runtime_error{"epoll_wait"};

        for (int n = 0; n < nfds; ++n)
        {
            auto socket = static_cast<Socket*>(events[n].data.ptr);
            std::cout << "epoll(" << socket->fd << ")\n";
            if (events[n].events & EPOLLIN)
                socket->resumeRecv();
            if (events[n].events & EPOLLOUT)
                socket->resumeSend();
        }
    }
}

void IOContext::attach(Socket* socket)
{
    struct epoll_event ev;
    auto io_state = EPOLLIN | EPOLLET;
    ev.events = io_state;
    ev.data.ptr = socket;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, socket->fd, &ev) == -1)
        throw std::runtime_error{"epoll_ctl: attach"};
    socket->io_state_ = io_state;
}

void IOContext::mod(Socket* socket)
{
    struct epoll_event ev;
    auto io_state = socket->io_new_state_;
    if (socket->io_state_ == io_state)
        return;
    ev.events = io_state;
    ev.data.ptr = socket;
    if (epoll_ctl(efd, EPOLL_CTL_MOD, socket->fd, &ev) == -1)
        throw std::runtime_error{"epoll_ctl: mod"};
    socket->io_state_ = io_state;
}

void IOContext::watchConnect(Socket* socket)
{
    socket->io_new_state_ = socket->io_state_ | EPOLLIN|EPOLLOUT|EPOLLERR|EPOLLHUP | EPOLLET;
    mod(socket);
}

void IOContext::watchRead(Socket* socket)
{
    socket->io_new_state_ = socket->io_state_ | EPOLLIN;
    mod(socket);
}

void IOContext::unwatchRead(Socket* socket)
{
    socket->io_new_state_ = socket->io_state_ & ~EPOLLIN;
    mod(socket);
}

void IOContext::watchWrite(Socket* socket)
{
    socket->io_new_state_ = socket->io_state_ | EPOLLOUT;
    mod(socket);
}

void IOContext::unwatchWrite(Socket* socket)
{
    socket->io_new_state_ = socket->io_state_ & ~EPOLLOUT;
    mod(socket);
}

void IOContext::detach(Socket* socket)
{
    if (epoll_ctl(efd, EPOLL_CTL_DEL, socket->fd, nullptr) == -1) {
        perror("epoll_ctl: detach");
        exit(EXIT_FAILURE);
    }
}
