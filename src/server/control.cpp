#include "control.h"

extern Config<confServer> config;

std::task<> Control::initControl()
{
    std::cout <<"initControl.............."<< std::endl;
    //---------------------------------------
    writer().resume;
    // //---------------------------------------
    authResp authResp_ = {"xxxxxx"};

    // //---------------------------------------
    // reader().resume;
    // manager().resume;
    // stopper().resume;
}

void Control::registerTunnel()
{
    
}

std::task<> Control::writer()
{
    std::cout <<"启动writer"<< std::endl;
    char sndBuff[SND_BUFF_SIZE];
    Msg msg;
    while (true)
    {
        msg = co_await out.read();
        co_await Msg::writeMsg(socket, sndBuff, &msg);
    }
}

std::task<> Control::reader()
{
    std::cout <<"启动reader"<< std::endl;
}

std::task<> Control::manager()
{
    std::cout <<"启动manager"<< std::endl;
}

std::task<> Control::stopper()
{
    std::cout <<"启动stopper"<< std::endl;
}

std::task<> Control::controlCoRoutine()
{
    char rcvBuff[RCV_BUFF_SIZE];
    ssize_t nbRcved = 0;
    while (true)
    {
        Msg msg;
        co_await Msg::readMsg(socket, rcvBuff, &nbRcved, &msg);
        switch(msg.type)
        {
        case msgType::Auth :
            {
                // std::cout <<"auth_user:"<< std::static_pointer_cast<auth>(msg.msg_)->user << std::endl;
                // std::cout <<"auth_password:"<<  std::static_pointer_cast<auth>(msg.msg_)->password << std::endl;
                auth_ = *(std::static_pointer_cast<auth>(msg.msg_));
                auto got = (config.conf)->user.find(std::static_pointer_cast<auth>(msg.msg_)->user);
                if ((got != (config.conf)->user.end()) && 
                    (got->second == std::static_pointer_cast<auth>(msg.msg_)->password))
                {
                    //认证成功
                    std::cout <<"认证成功"<< std::endl;
                    initControl();
                } else
                {
                    //认证失败
                    std::cout <<"认证失败"<< std::endl;
                    close();
                    co_return;
                }
                break;
            }
        case msgType::RegProxy :
            {
                break;
            }
        default:
            close();
            break;
        }
    }
}

void Control::close()
{
    
}