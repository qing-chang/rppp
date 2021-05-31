#include "tunnel.h"

extern Config<confClient> config;
extern IOContext io_context;

std::task<> Proxy::NewProxy()
{
    //------------------------------发送到中转服务器的链接----------------------------------------------------
    socketIn = std::shared_ptr<Socket>(new Socket{io_context, 0});
    int c = co_await socketIn->connect(config.conf->serverAddr, config.conf->serverPort);
    if(c != 0)
    {
        std::cout << "connect fail" << std::endl;
    }else
    {
        std::cout << "connect succeed" << std::endl;
    }
    //------------------------------发送RegProxy----------------------------------------------------

    //------------------------------接收startProxy----------------------------------------------------

    //------------------------------发送到内容服务器的链接----------------------------------------------------
    
    //------------------------------中转信息---------------------------------------------------------

}

std::task<> Tunnel::NewTunnel()
{
    // //------------------------------发送NewTunnel---------------------------------------------------------
    // char sndBuff[SND_BUFF_SIZE];
    // Msg msg;
    // msg.type = msgType::NewTunnel;
    // std::shared_ptr<newTunnel> newTunnel_(new newTunnel{std::to_string(remotePort)});
    // msg.msg_ = std::static_pointer_cast<void>(newTunnel_);
    // co_await _msg_::writeMsg(control->socket, sndBuff, &msg);
    // //------------------------------开始监听用户链接-------------------------------------------------------
    // tunnelListener().resume();
}