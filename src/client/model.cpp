#include "model.h"
#include "../msg/msg.h"
#include <iostream>

extern Config<confClient> config;
extern IOContext io_context;

void ClientModel::Run()
{
    control().resume();
}

std::task<> ClientModel::control()
{
    //---------------------------------------------------------------------------------------------------------
    socket =  std::shared_ptr<Socket>(new Socket{io_context, 0});
    int c = co_await socket->connect(config.conf->serverAddr, config.conf->serverPort);
    if(c != 0)
    {
        std::cout << "connect fail" << std::endl;
    }else
    {
        std::cout << "connect succeed" << std::endl;
    }
    //--------------------------------------------------------------------------------------------------
    char sndBuff[SND_BUFF_SIZE];
    char rcvBuff[RCV_BUFF_SIZE];
    Msg msg;
    //---------------------------------------发生auth-----------------------------------------------------------
    msg.type = msgType::Auth;
    std::shared_ptr<auth> auth_(new auth{(config.conf)->userName, (config.conf)->password});
    msg.msg_ = std::static_pointer_cast<void>(auth_);
    co_await Msg::writeMsg(socket, sndBuff, &msg);
    //---------------------------------------接收auth-----------------------------------------------------------
    ssize_t nbRcved = 0;
    while(true)
    {
        co_await Msg::readMsg(socket, rcvBuff, &nbRcved, &msg);
        if(msg.type == msgType::AuthResp)
        {
            std::cout << "认证成功........." << std::endl;
            break;
        }
    }
    //---------------------------------------发送ReqTunnel-------------------------------------------------------
    msg.type = msgType::ReqTunnel;
    std::shared_ptr<reqTunnel> reqTunnel_;
    for(auto t:(config.conf)->tunnel)
    {
        reqTunnel_ = std::shared_ptr<reqTunnel>(new reqTunnel{t.type, t.name, t.remotePort});
        msg.msg_ = std::static_pointer_cast<void>(reqTunnel_);
        co_await Msg::writeMsg(socket, sndBuff, &msg);
    }
}