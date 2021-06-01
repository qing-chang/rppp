#pragma once

#include <coroutine>
#include <vector>
#include "model.h"
#include "../frame/task.h"
#include "../channel/channel.h"
#include "../socket/socket.h"
#include "../msg/msg.h"
#include "../conf/conf.h"

class ClientModel;

class Proxy
{
public:
    std::shared_ptr<Socket> socketDown;
    std::shared_ptr<Socket> socketUp;
    ClientModel *clientModel;

    Proxy(ClientModel *clientModel):clientModel(clientModel){}
    std::task<> NewProxy();
};

class Tunnel
{
public:
    int remotePort;
    std::string localAddr;
	int localPort;
    ClientModel *clientModel;
    std::vector<std::shared_ptr<Proxy>> proxys;

    Tunnel(ClientModel *clientModel):clientModel(clientModel){}
    void NewTunnel();
};