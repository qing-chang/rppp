#include "model.h"
#include "../msg/msg.h"
#include <iostream>

extern Config<confClient> config;
extern IOContext io_context;

void ClientModel::Run()
{
    control().resume();
}

std::task<> ClientModel::control()
{
    std::cout<<"control"<<"\n";
    //---------------------------------------------------------------------------------------------------------
    socket =  std::shared_ptr<Socket>(new Socket{io_context, 0});
    int c = co_await socket->connect(config.conf->serverAddr, config.conf->serverPort);
    if(c != 0)
    {
        std::cout << "connect fail" << std::endl;
    }else
    {
        std::cout << "connect succeed" << std::endl;
    }
    //---------------------------------------发生auth-----------------------------------------------------------
    unsigned char sndBuff[1024];
    iguana::string_stream auth_ss;
    auth auth_ = {"chang", "123456"};
	iguana::json::to_json(auth_ss, auth_);
    auto json_str = auth_ss.str();
    json_str.copy(sndBuff, json_str.length(), 4);
    msgHdr *h = (msgHdr *)sndBuff;
    *h = {msgType.Auth, json_str.length()};
    ssize_t nbSend = json_str.length() + 4;
    ssize_t nbSended = 0;
    while (nbSended < nbSend)
    {
        ssize_t res = co_await socket->send(sndBuff + nbSended, nbSend - nbSended);
        if (res <= 0)
            break;
        nbSended += res;
    }
    //---------------------------------------接收auth-----------------------------------------------------------

}