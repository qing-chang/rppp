#include "tunnel.h"

std::task<> Tunnel::NewTunnel()
{
    //------------------------------发送NewTunnel-----------------------------------------------------------
    char sndBuff[SND_BUFF_SIZE];
    Msg msg;
    msg.type = msgType::NewTunnel;
    std::shared_ptr<newTunnel> newTunnel_(new newTunnel{"newTunnel"});
    msg.msg_ = std::static_pointer_cast<void>(newTunnel_);
    co_await _msg_::writeMsg(control->socket, sndBuff, &msg);
    //------------------------------开始监听用户链接---------------------------------------------------------
    
}