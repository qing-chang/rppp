#pragma once

#include <vector>
#include "../channel/channel.h"
#include "tunnel.h"
#include "../socket/socket.h"
#include "../msg/msg.h"
#include "../conf/conf.h"

class Tunnel;

class Control //: public Msg
{
public:
    std::shared_ptr<Socket> socket;
    Channel<Msg> in, out;
    auth auth_;
    std::vector<std::shared_ptr<Tunnel>> tunnels;

    Control(){}
    std::task<> initControl();
    void registerTunnel();
    std::task<> writer();
    std::task<> reader();
    std::task<> manager();
    std::task<> stopper();
    std::task<> controlCoRoutine();
    void close();
};