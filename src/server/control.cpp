#include "control.h"

extern Config<confServer> config;

std::task<> Control::initControl()
{
    std::cout <<"initControl.............."<< std::endl;
    //----------------------------------------------
    writer().resume();
    //-----------------发送AuthResp------------------
    char sndBuff[SND_BUFF_SIZE];
    Msg msg;
    msg.type = msgType::AuthResp;
    std::shared_ptr<authResp> authResp_(new authResp{"xxxxxx"});
    msg.msg_ = std::static_pointer_cast<void>(authResp_);
    co_await out.write(msg);
    //----------------发送ReqProxy----------------------
    msg.type = msgType::ReqProxy;
    std::shared_ptr<reqProxy> reqProxy_(new reqProxy{"xxxxxx"});
    msg.msg_ = std::static_pointer_cast<void>(reqProxy_);
    // co_await out.write(msg);
    //---------------------------------------
    manager().resume();
    reader().resume();
    // stopper().resume();
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
        co_await _msg_::writeMsg(socket, sndBuff, &msg);
    }
}

std::task<> Control::reader()
{
    std::cout <<"启动reader"<< std::endl;
    char rcvBuff[RCV_BUFF_SIZE];
    ssize_t nbRcved = 0;
    Msg msg;
    while (true)
    {
        co_await _msg_::readMsg(socket, rcvBuff, &nbRcved, &msg);
        co_await in.write(msg);
    }
}

std::task<> Control::manager()
{
    std::cout <<"启动manager"<< std::endl;
    char sndBuff[SND_BUFF_SIZE];
    Msg msg;
    while (true)
    {
        msg = co_await in.read();
        switch(msg.type)
        {
        case msgType::ReqTunnel :
            std::cout <<"收到ReqTunnel"<< std::endl;
            std::shared_ptr<Tunnel> tunnel = std::shared_ptr<Tunnel>(new Tunnel{this});
            std::shared_ptr<reqTunnel> reqTunnel_ = std::static_pointer_cast<reqTunnel>(msg.msg_);
            tunnel->remotePort = reqTunnel_->remotePort;
            tunnel->NewTunnel().resume();
            tunnels.push_back(tunnel);
            break;
        case msgType::Ping :
            std::cout <<"收到Ping"<< std::endl;
            msg.type = msgType::Pong;
            co_await _msg_::writeMsg(socket, sndBuff, &msg);
            break;
        }
    }
}

std::task<> Control::stopper()
{
    std::cout <<"启动stopper"<< std::endl;
}

std::task<> Control::controlCoRoutine()
{
    char rcvBuff[RCV_BUFF_SIZE];
    ssize_t nbRcved = 0;
    Msg msg;
    co_await _msg_::readMsg(socket, rcvBuff, &nbRcved, &msg);
    switch(msg.type)
    {
    case msgType::Auth :
        {
            auth_ = *(std::static_pointer_cast<auth>(msg.msg_));
            auto got = (config.conf)->user.find(auth_.user);
            if ((got != (config.conf)->user.end()) && (got->second == auth_.password))
            {
                //认证成功
                std::cout <<"认证成功"<< std::endl;
                initControl().resume();
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

void Control::close()
{
    
}