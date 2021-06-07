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
class Proxy;

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
    std::task<> tunnelManager(std::shared_ptr<Socket> socket);
};