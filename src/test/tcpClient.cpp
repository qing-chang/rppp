#include "../frame/io_context.h"
#include "../socket/socket.h"
#include "../frame/task.h"

IOContext io_context;
std::string host;
int port;

std::task<> readWrite(std::shared_ptr<Socket> socket)
{
     while(true)
     {
          int c = co_await socket->connect(host, port);
          if(c == 0)
          {
               std::cout << "连接成功" << std::endl;
               break;
          }else
          {
               std::cout << "连接失败，再次尝试。" << std::endl;
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
/*********************************************************************************************
 * 参数：第1个参数为服务器IP地址，
 *       第2个参数为服务器端口号。
 * 例如：./tcpClient 192.168.1.50 8888
*********************************************************************************************/
int main(int argc, char * argv[])
{
     //解析参数
     if(argc <=2)
     {
          return -1;
     }
     host = argv[1];
     port = std::atoi(argv[2]);

     io_context.init();
     std::shared_ptr<Socket> socket(new Socket{io_context, 0});
     readWrite(socket).resume();
     io_context.run();
     return 0;
}