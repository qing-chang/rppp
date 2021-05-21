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
        char buffer[1024] = {0};
        ssize_t nbRecv = co_await socket->recv(buffer, sizeof buffer);
        ssize_t nbSend = 0;
        while (nbSend < nbRecv)
        {
            ssize_t res = co_await socket->send(buffer, sizeof buffer);
            if (res <= 0)
                break;
            nbSend += res;
        }
        std::cout << "DONE (" << nbRecv << ")" << '\n';
        if (nbRecv <= 0)
            run = false;
        else
            printf("%s\n", buffer);
    }
}