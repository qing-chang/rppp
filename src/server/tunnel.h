#pragma once

#include <coroutine>
#include "control.h"
#include "../frame/task.h"

class Control;

class Tunnel
{
public:
    int remotePort;
    Control *control;

    Tunnel(Control *control):control(control){}
    std::task<> NewTunnel();
    
};