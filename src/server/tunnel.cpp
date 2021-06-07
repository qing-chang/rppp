#include "tunnel.h"

extern IOContext io_context;

std::task<> Tunnel::NewTunnel()
{
    //------------------------------发送NewTunnel-----------------------------------------------------------
    char sndBuff[SND_BUFF_SIZE];
    Msg msg;
    msg.type = msgType::NewTunnel;
    std::shared_ptr<newTunnel> newTunnel_(new newTunnel{remotePort});
    msg.msg_ = std::static_pointer_cast<void>(newTunnel_);
    co_await _msg_::writeMsg(control->socket, sndBuff, &msg);
    //------------------------------开始监听用户链接---------------------------------------------------------
    tunnelListener().resume();
}

std::task<> Tunnel::tunnelListener()
{
    std::shared_ptr<Socket> tunnelListener_ = std::shared_ptr<Socket>(new Socket{io_context, remotePort});
    while (true)
    {
        auto socket = co_await tunnelListener_->accept();
        tunnelManager(socket).resume();
    }
}

std::task<> Tunnel::tunnelManager(std::shared_ptr<Socket> socket)
{
    if(std::shared_ptr<Proxy> proxy = co_await control->proxy_bak.read())
    {
        //------------------------发送StartProxy-----------------------------------------------------------
        char sndBuff[SND_BUFF_SIZE];
        Msg msg;
        msg.type = msgType::StartProxy;
        std::shared_ptr<startProxy> startProxy_(new startProxy{remotePort});
        msg.msg_ = std::static_pointer_cast<void>(startProxy_);
        co_await _msg_::writeMsg(proxy->socketUp, sndBuff, &msg);
        //------------------------重新申请proxy------------------------------------------------------------
        msg.type = msgType::ReqProxy;
        std::shared_ptr<reqProxy> reqProxy_(new reqProxy{control->id});
        msg.msg_ = std::static_pointer_cast<void>(reqProxy_);
        co_await  _msg_::writeMsg(control->socket, sndBuff, &msg);
        //-----------------------------------------------------------------------------------------
        proxy->tunnel = std::shared_ptr<Tunnel>(this);
        proxy->socketDown = socket;
        proxy->forward(proxy->socketDown, proxy->socketUp).resume();
        proxy->forward(proxy->socketUp, proxy->socketDown).resume();
        proxys.push_back(proxy);
    }
}