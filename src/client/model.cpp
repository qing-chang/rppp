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
    char sndBuff[1024];
    auth auth_ = {"chang", "123456"};
    co_await Msg::writeMsg(socket, sndBuff, msgType::Auth, &auth_);
    //---------------------------------------接收auth-----------------------------------------------------------
    
}