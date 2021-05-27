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
    std::cout<<"control"<<"\n";
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
    //---------------------------------------发生auth-----------------------------------------------------------
    char sndBuff[SND_BUFF_SIZE];
    Msg msg;
    msg.type = msgType::Auth;
    std::shared_ptr<auth> auth_(new auth{(config.conf)->userName, (config.conf)->password});
    msg.msg_ = std::static_pointer_cast<void>(auth_);
    co_await Msg::writeMsg(socket, sndBuff, &msg);
    //---------------------------------------接收auth-----------------------------------------------------------
    char rcvBuff[RCV_BUFF_SIZE];
    ssize_t nbRcved = 0;
    Msg msg;
    while(true)
    {
        co_await Msg::readMsg(socket, rcvBuff, &nbRcved, &msg);

    }
}