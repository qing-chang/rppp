#pragma once

#include <vector>
#include "../channel/channel.h"
#include "tunnel.h"
#include "../socket/socket.h"
#include "../msg/msg.h"

class Control : public Msg
{
public:
    std::shared_ptr<Socket> socket;
    Msg msg;
    Channel<int> in;
    Channel<int> out;
    std::vector<Tunnel> tunnels;
    Control(){}
    void initControl();
    void registerTunnel();
    void writer();
    void reader();
    void manager();
    void stopper();
    std::task<> controlCoRoutine();
};