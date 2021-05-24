#include "control.h"

void Control::NewControl()
{
    
}

void Control::registerTunnel()
{
    
}

void Control::writer()
{
    
}

void Control::reader()
{
    
}

void Control::manager()
{
    
}

void Control::stopper()
{
    
}

std::task<> Control::controlCoRoutine()
{
    // ssize_t rsize,nbRecv;
    char rcvBuff[RCV_BUFF_SIZE];
    ssize_t nbRcved = 0;
    while (true)
    {
        // while(true)
        // {
        //     if(begin > end)
        //         rsize = begin-end;
        //     else
        //         rsize = BUFFER_SIZE-end;
        //     ssize_t nbRecv = co_await socket->recv(buffer+end, rsize);
        //     checkPack();
        //     if(floor != 0)
        //         continue;
        //     else
        // }
        co_await Msg::readMsg(socket, rcvBuff, &nbRcved);
        switch(((msgHdr *)rcvBuff)->type)
        {
        case msgType::Auth :
            // std::cout << "auth............" << std::endl;
            auth auth_;
            iguana::json::from_json0(auth_, rcvBuff + 4, ((msgHdr *)rcvBuff)->len - 4);
            std::cout <<"auth_user:"<< auth_.user << std::endl;
            std::cout <<"auth_password:"<< auth_.password << std::endl;
            break;
        }
        //---------------------------------------
        // char buffer[1024] = {0};
        // ssize_t nbRecv = co_await socket->recv(buffer, sizeof buffer);
        // ssize_t nbSend = 0;
        // while (nbSend < nbRecv)
        // {
        //     ssize_t res = co_await socket->send(buffer, sizeof buffer);
        //     if (res <= 0)
        //         break;
        //     nbSend += res;
        // }
        // std::cout << "DONE (" << nbRecv << ")" << '\n';
    }
}