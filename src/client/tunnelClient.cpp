#include "tunnelClient.h"

extern Config<confClient> config;
extern IOContext io_context;

std::task<> Proxy::NewProxy()
{
    //------------------------------发起到中转服务器的链接----------------------------------------------------
    socketDown = std::shared_ptr<Socket>(new Socket{io_context, 0});
    int c = co_await socketDown->connect(config.conf->serverAddr, config.conf->serverPort);
    if(c != 0)
    {
        std::cout << "connect fail" << std::endl;
    }else
    {
        std::cout << "connect succeed" << std::endl;
    }
    //------------------------------发送RegProxy----------------------------------------------------
    char sndBuff[SND_BUFF_SIZE];
    Msg msg;
    msg.type = msgType::RegProxy;
    std::shared_ptr<regProxy> regProxy_(new regProxy{clientModel->id});
    msg.msg_ = std::static_pointer_cast<void>(regProxy_);
    co_await _msg_::writeMsg(socketDown, sndBuff, &msg);
    //------------------------------接收startProxy----------------------------------------------------
    char rcvBuff[SND_BUFF_SIZE];
    ssize_t nbRcved = 0;
    while(true)
    {
        co_await _msg_::readMsg(socketDown, rcvBuff, &nbRcved, &msg);
        if(msg.type == msgType::StartProxy)
        {
            std::cout << "startProxy........." << std::endl;
            int remotePort = std::static_pointer_cast<startProxy>(msg.msg_)->remotePort;
            for(auto t:clientModel->tunnels)
            {
                if(t->remotePort == remotePort)
                {
                    tunnel = t;
                    t->proxys.push_back(std::shared_ptr<Proxy>(this));
                    break;
                }
            }
            break;
        }
    }
    //------------------------------发起到内容服务器的链接----------------------------------------------------
    socketUp = std::shared_ptr<Socket>(new Socket{io_context, 0});
    c = co_await socketUp->connect(tunnel->localAddr, tunnel->localPort);
    if(c != 0)
    {
        std::cout << "connect fail" << std::endl;
    }else
    {
        std::cout << "connect succeed" << std::endl;
    }
    //------------------------------中转信息---------------------------------------------------------
    forward(socketDown, socketUp).resume();
    forward(socketUp, socketDown).resume();
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

void Tunnel::NewTunnel()
{
    for(auto t:(config.conf)->tunnel)
    {
        if(t.remotePort == remotePort)
        {
            localAddr = t.localAddr;
            localPort = t.localPort;
            break;
        }
    }
}