#pragma once

#include "../frame/io_context.h"
#include "../socket/socket.h"
#include "../conf/conf.h"
#include "../frame/time.h"
#include "tunnel.h"

class ClientModel;
class Tunnel;
class Proxy;

typedef std::task<> (ClientModel::*callbBack)(timerNode *);

class ClientModel
{
public:
    std::shared_ptr<Socket> socket;
    std::vector<std::shared_ptr<Tunnel>> tunnels;
    std::vector<std::shared_ptr<Proxy>> proxys;

    void Run();
    std::task<> control();
    timerNode *addTimer(int duration, callbBack cb);
    int delTimer(timerNode *tn);
    std::task<> heartbeat(timerNode *tn);
};

struct timerTick
{
    timerTick(timerNode *tn):tn(tn){}
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> h)
    {
        if (clock() > tn->time)
            h.resume();
    }
    void await_resume()
    {}
    timerNode *tn;
};