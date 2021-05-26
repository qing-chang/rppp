#include "control.h"

extern Config<confServer> config;

void Control::initControl()
{
    std::cout <<"initControl:"<< std::endl;
    authResp authResp_ = {"xxxxxx"};
    
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
                // auth auth_;
                // iguana::json::from_json0(auth_, rcvBuff + 4, ((msgHdr *)rcvBuff)->len - 4);
                std::cout <<"auth_user:"<< std::static_pointer_cast<auth>(msg.msg_)->user << std::endl;
                std::cout <<"auth_password:"<<  std::static_pointer_cast<auth>(msg.msg_)->password << std::endl;
                // auto got = (config.conf)->user.find(auth_.user);
                // if ((got != (config.conf)->user.end()) && (got->second == auth_.password))
                // {
                //     initControl();
                // } else
                // {
                //     co_return;
                // }
                break;
            }
        case msgType::RegProxy :
            {
                regProxy regProxy_;
                iguana::json::from_json0(regProxy_, rcvBuff + 4, ((msgHdr *)rcvBuff)->len - 4);
                break;
            }
        default:
            break;
        }
        //---------------------------------------
        // char buffer[1024] = {0};
        // ssize_t nbRecv = co_await socket->recv(buffer, sizeof buffer);
        // ssize_t nbSend = 0;
        // while (nbSend < nbRecv)
        // {
        //     ssize_t res = co_await socket->send(buffer, sizeof buffer);
        //     if (res <= 0)
        //         break;
        //     nbSend += res;
        // }
    }
}