#include "control.h"

extern Config<confServer> config;

inline std::string genRandomId()
{
    return std::string{"123456"};
}

std::task<> Control::initControl()
{
    std::cout <<"initControl.............."<< std::endl;
    //----------------------------------------------
    writer().resume();
    //-----------------发送AuthResp------------------
    char sndBuff[SND_BUFF_SIZE];
    Msg msg;
    msg.type = msgType::AuthResp;
    id = genRandomId();
    std::shared_ptr<authResp> authResp_(new authResp{id});
    msg.msg_ = std::static_pointer_cast<void>(authResp_);
    co_await out.write(msg);
    //----------------发送ReqProxy----------------------
    msg.type = msgType::ReqProxy;
    std::shared_ptr<reqProxy> reqProxy_(new reqProxy{id});
    msg.msg_ = std::static_pointer_cast<void>(reqProxy_);
    co_await out.write(msg);
    //---------------------------------------
    manager().resume();
    reader().resume();
    // stopper().resume();
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
            {
                std::cout <<"收到ReqTunnel"<< std::endl;
                std::shared_ptr<Tunnel> tunnel(new Tunnel{this});
                tunnel->remotePort = std::static_pointer_cast<reqTunnel>(msg.msg_)->remotePort;
                tunnel->NewTunnel().resume();
                tunnels.push_back(tunnel);
                break;
            }
        case msgType::Ping :
            {
                std::cout <<"收到Ping"<< std::endl;
                lastPing = clock();
                msg.type = msgType::Pong;
                co_await out.write(msg);
                break;
            }
        }
    }
}

std::task<> Control::stopper()
{
    std::cout <<"启动stopper"<< std::endl;
}

void Control::close()
{
    
}