#pragma once

#include <coroutine>
#include <vector>
#include "control.h"
#include "../frame/task.h"
#include "../channel/channel.h"
#include "../socket/socket.h"
#include "../msg/msg.h"
#include "../conf/conf.h"

class Control;
class Tunnel;

class Proxy
{
public:
    std::shared_ptr<Socket> socketDown;
    std::shared_ptr<Socket> socketUp;
    std::shared_ptr<Control> control;
    std::shared_ptr<Tunnel>  tunnel;
    std::task<> forward(std::shared_ptr<Socket> socketIn, std::shared_ptr<Socket> socketOut);
};

class Tunnel
{
public:
    int remotePort;
    Control *control;
    std::shared_ptr<Socket> tunnelListener_;
    std::vector<std::shared_ptr<Proxy>> proxys;

    Tunnel(Control *control):control(control){}
    std::task<> NewTunnel();
    std::task<> tunnelListener();
};