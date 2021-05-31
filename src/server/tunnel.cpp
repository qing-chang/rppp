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
        std::shared_ptr<Proxy> proxy = std::shared_ptr<Proxy>(new Proxy);
        proxy->socketIn = co_await tunnelListener_->accept();
        // proxy->controlCoRoutine().resume();
        proxys.push_back(proxy);
    }
}