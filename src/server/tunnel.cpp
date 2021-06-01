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
        if(std::shared_ptr<Proxy> proxy = *(control->proxy_bak.begin()))
        {
        // control->proxy_bak.erase(control->proxy_bak.begin());
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
        proxy->socketDown = socket;
        proxy->forward(proxy->socketDown, proxy->socketUp).resume();
        proxy->forward(proxy->socketUp, proxy->socketDown).resume();
        proxys.push_back(proxy);
        }
    }
}

std::task<> Proxy::forward(std::shared_ptr<Socket> socketIn, std::shared_ptr<Socket> socketOut)
{
    char rcvBuff[RCV_BUFF_SIZE];
    ssize_t rcv, nbSended, snd;
    while (true)
    {
        rcv = co_await socketIn->recv(rcvBuff, RCV_BUFF_SIZE);
        nbSended = 0;
        while (nbSended < rcv)
        {
            snd = co_await socketOut->send(rcvBuff + nbSended, rcv - nbSended);
            if (snd <= 0)
                break;
            nbSended += snd;
        }
    }
}