#include "control.h"

#define RCV_BUFF_SIZE 1024

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
    bool run = true;
    ssize_t rsize,nbRecv;
    while (run)
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
        //     {
        //         readMsg();
        //         switch (type)
        //         {
        //         case msgTypeAuth:
        //             /* code */
        //             break;
                
        //         default:
        //             break;
        //         }
        //     }
        // }
        // run = co_await inside_loop(*socket);
        char rcvBuff[RCV_BUFF_SIZE];
        ssize_t nbRcved, res;
start:
        nbRcved = 0;
        while(nbRcved < 4)
        {
            res = co_await socket->recv(rcvBuff, RCV_BUFF_SIZE - nbRcved);
            nbRcved += res;
        }
parse_hdr:
        msgHdr *h = (msgHdr *)rcvBuff;
        while(nbRcved < h->len)
        {
            res = co_await socket->recv(rcvBuff, RCV_BUFF_SIZE - nbRcved);
            nbRcved += res;
        }
        auth auth_;
        iguana::json::from_json0(auth_, rcvBuff + 4, h->len - 4);
        std::cout <<"auth_user:"<< auth_.user << std::endl;
        std::cout <<"auth_password:"<< auth_.password << std::endl;
      
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
        // if (nbRecv <= 0)
        //     run = false;
        // else
        //     printf("%s\n", buffer);
    }
}