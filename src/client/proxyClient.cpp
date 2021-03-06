#include "proxyClient.h"

extern Config<confClient> config;
extern IOContext io_context;

std::task<> Proxy::NewProxy()
{
    //------------------------------发起到中转服务器的链接----------------------------------------------------
    socketDown = std::shared_ptr<Socket>(new Socket{io_context, 0});
    while(true)
    {
        int c = co_await socketDown->connect(config.conf->serverAddr, config.conf->serverPort);
        if(c == 0)
        {
            std::cout << "connect succeed" << std::endl;
            break;
        }else
        {
            std::cout << "connect fail,try again." << std::endl;
        }
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
            auto tunnelIter = find_if(clientModel->tunnels.begin(), clientModel->tunnels.end(),
                [&remotePort](auto t) {
                    return (t->remotePort == remotePort);
                });
            if(tunnelIter !=  clientModel->tunnels.end())
            {
                // std::cout <<"查找到匹配的Tunnel..."<< std::endl;
                tunnel = *tunnelIter;
                (*tunnelIter)->proxys.push_back(std::shared_ptr<Proxy>(this));
            }
            break;
        }
    }
    //------------------------------发起到内容服务器的链接--------------------------------------------------
    socketUp = std::shared_ptr<Socket>(new Socket{io_context, 0});
    while(true)
    {
        int c = co_await socketUp->connect(tunnel->localAddr, tunnel->localPort);
        if(c == 0)
        {
            std::cout << "connect succeed" << std::endl;
            //------------------------------中转信息---------------------------------------------------------
            forward(socketDown, socketUp).resume();
            forward(socketUp, socketDown).resume();
            break;
        }else
        {
            std::cout << "connect fail" << std::endl;
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