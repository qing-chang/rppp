#pragma once

#include <vector>
#include "../channel/channel.h"
#include "tunnel.h"
#include "../socket/socket.h"
#include "../msg/msg.h"
#include "../conf/conf.h"
#include "../frame/time.h"

class Tunnel;
class Proxy;

class Control
{
public:
    std::string id;
    std::shared_ptr<Socket> socket;
    Channel<Msg> in, out;
    auth auth_;
    std::vector<std::shared_ptr<Tunnel>> tunnels;
    std::vector<std::shared_ptr<Proxy>> proxy_bak;
    timerNode *tn;
    clock_t lastPing;

    Control(){}
    std::task<> initControl();
    // void registerTunnel();
    std::task<> writer();
    std::task<> reader();
    std::task<> manager();
    std::task<> stopper();
    // std::task<> controlCoRoutine();
    void close();
};