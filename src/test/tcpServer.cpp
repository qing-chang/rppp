#include "../frame/io_context.h"
#include "../socket/socket.h"
#include "../frame/task.h"

IOContext io_context;

std::task<> readWrite(std::shared_ptr<Socket> socket)
{
     char rcvBuff[RCV_BUFF_SIZE];
     ssize_t nbRcved = 0;
     ssize_t res, nbSended, s;
     while (true)
     {
          res = co_await socket->recv(rcvBuff, RCV_BUFF_SIZE - 1);
          rcvBuff[res] = 0;
          if(res !=0)
          {
               std::cout <<"收到： "<< rcvBuff <<std::endl;
          }else{
               std::cout<< "对端已关闭连接，关闭本端socket"<< std::endl;
               io_context.detach(socket.get());
               close(socket->fd);
               co_return;
          }
          nbSended = 0;
          while (nbSended < res)
          {
               s = co_await socket->send(rcvBuff + nbSended, res - nbSended);
               if (s <= 0)
                    break;
               nbSended += s;
          }
     }
}

std::task<> listen(std::shared_ptr<Socket> listener)
{
    while (true)
    {
        std::shared_ptr<Socket> socket = co_await listener->accept();
        std::cout <<"有新用户链入"<< std::endl;
        readWrite(socket).resume();
    }
}

int main()
{
    io_context.init();
    std::shared_ptr<Socket> listener(new Socket{io_context, 8888});
    listen(listener).resume();
    io_context.run();
}