#include "control.h"

extern Config<confServer> config;

void Control::initControl()
{
    std::cout <<"initControl.............."<< std::endl;
    authResp authResp_ = {"xxxxxx"};
    //---------------------------------------
}

void Control::registerTunnel()
{
    
}

void Control::writer()
{
    
}

void Control::reader()
{
    
}

void Control::manager()
{
    
}

void Control::stopper()
{
    
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
                auto got = (config.conf)->user.find(std::static_pointer_cast<auth>(msg.msg_)->user);
                if ((got != (config.conf)->user.end()) && 
                    (got->second == std::static_pointer_cast<auth>(msg.msg_)->password))
                {
                    //认证成功
                    std::cout <<"/认证成功"<< std::endl;
                    initControl();
                } else
                {
                    //认证失败
                    std::cout <<"/认证失败"<< std::endl;
                    co_return;
                }
                break;
            }
        case msgType::RegProxy :
            {
                break;
            }
        default:
            break;
        }
    }
}