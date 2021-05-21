#pragma once

#include "../frame/io_context.h"
#include "../socket/socket.h"
#include "../conf/conf.h"

class ClientModel
{
public:
    std::shared_ptr<Socket> socket;
    void Run();
    std::task<> control();
};