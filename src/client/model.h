#pragma once

#include "../frame/io_context.h"
#include "../socket/socket.h"
#include "../conf/conf.h"
#include "../frame/time.h"

class ClientModel
{
public:
    std::shared_ptr<Socket> socket;
    void Run();
    std::task<> control();
};
    std::task<> heartbeat(timerNode *tn);//, ClientModel *cm);

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