#include "../frame/io_context.h"
#include "../socket/socket.h"
#include "../frame/task.h"

IOContext io_context;

std::task<> readWrite(std::shared_ptr<Socket> socket)
{
     while(true)
     {
          int c = co_await socket->connect("127.0.0.1", 8888);
          if(c == 0)
          {
               std::cout << "connect succeed" << std::endl;
               break;
          }else
          {
               std::cout << "connect fail,try again." << std::endl;
          }
     }
     char rcvBuff[RCV_BUFF_SIZE];
     ssize_t nbRcved = 0;
     ssize_t res, nbSended, s;
     while (true)
     {
          std::cout <<"输入： " <<std::endl;
          std::cin>> rcvBuff;
          nbSended = 0;
          res = strlen(rcvBuff);
          while (nbSended < res)
          {
               s = co_await socket->send(rcvBuff + nbSended, res - nbSended);
               if (s <= 0)
                    break;
               nbSended += s;
          }
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
     }
}

int main()
{
     io_context.init();
     std::shared_ptr<Socket> socket(new Socket{io_context, 0});
     readWrite(socket).resume();
     io_context.run();
}