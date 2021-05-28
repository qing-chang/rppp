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
    Msg msg_s, msg_r;
    //---------------------------------------发送auth-----------------------------------------------------------
    msg_s.type = msgType::Auth;
    std::shared_ptr<auth> auth_(new auth{(config.conf)->userName, (config.conf)->password});
    msg_s.msg_ = std::static_pointer_cast<void>(auth_);
    co_await _msg_::writeMsg(socket, sndBuff, &msg_s);
    //---------------------------------------接收auth-----------------------------------------------------------
    ssize_t nbRcved = 0;
    while(true)
    {
        co_await _msg_::readMsg(socket, rcvBuff, &nbRcved, &msg_r);
        if(msg_r.type == msgType::AuthResp)
        {
            std::cout << "认证成功........." << std::endl;
            break;
        }
    }
    //---------------------------------------发送ReqTunnel-------------------------------------------------------
    msg_s.type = msgType::ReqTunnel;
    std::shared_ptr<reqTunnel> reqTunnel_;
    for(auto t:(config.conf)->tunnel)
    {
        reqTunnel_ = std::shared_ptr<reqTunnel>(new reqTunnel{t.type, t.name, t.remotePort});
        msg_s.msg_ = std::static_pointer_cast<void>(reqTunnel_);
        co_await _msg_::writeMsg(socket, sndBuff, &msg_s);
    }
    //---------------------------------------启动heartbeat-------------------------------------------------------
    // heartbeat().resume();
    //---------------------------------------主循环-------------------------------------------------------
    while(true)
    {
        co_await _msg_::readMsg(socket, rcvBuff, &nbRcved, &msg_r);
        switch(msg_r.type)
        {
        case msgType::ReqProxy :
            {
                std::cout <<"收到ReqProxy"<< std::endl;
                std::shared_ptr<reqProxy> reqProxy_ = std::static_pointer_cast<reqProxy>(msg_r.msg_);

                break;
            }
        case msgType::Pong :
            {
                std::cout <<"收到Pong"<< std::endl;

                break;
            }
        case msgType::NewTunnel :
            {
                std::cout <<"收到NewTunnel"<< std::endl;

                break;
            }
        default :
            {

                break;
            }
        }
    }
}

std::task<> ClientModel::heartbeat()
{

}