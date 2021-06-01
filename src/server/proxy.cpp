#include "proxy.h"

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