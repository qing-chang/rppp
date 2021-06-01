// #include <vector>
#include "../conf/conf.h"
#include "../frame/io_context.h"
#include "../socket/socket.h"
#include "../frame/task.h"
#include "../channel/channel.h"
#include "control.h"

Channel<int> channel;
std::vector<std::shared_ptr<Control>> controlRegistry;
Config<confServer> config;
IOContext io_context;

std::task<> controlProxyListener(std::shared_ptr<Socket> listener)
{
    while (true)
    {
        std::shared_ptr<Socket> socket = co_await listener->accept();
        char rcvBuff[RCV_BUFF_SIZE];
        ssize_t nbRcved = 0;
        Msg msg;
        co_await _msg_::readMsg(socket, rcvBuff, &nbRcved, &msg);
        switch(msg.type)
        {
        case msgType::Auth :
            {
                std::shared_ptr<Control> control(new Control);
                control->auth_ = *(std::static_pointer_cast<auth>(msg.msg_));
                auto got = (config.conf)->user.find(control->auth_.user);
                if ((got != (config.conf)->user.end()) && (got->second == control->auth_.password))
                {
                    //认证成功
                    std::cout <<"认证成功"<< std::endl;
                    control->socket = socket;
                    controlRegistry.push_back(control);
                    control->initControl().resume();
                } else
                {
                    //认证失败
                    std::cout <<"认证失败"<< std::endl;
                    co_return;
                }
                break;
            }
        case msgType::RegProxy :
            {
                std::cout <<"收到RegProxy..."<< std::endl;
                std::shared_ptr<Proxy> proxy(new Proxy);
                proxy->socketUp = socket;
                for(auto c:controlRegistry)
                {
                    if(c->id == std::static_pointer_cast<regProxy>(msg.msg_)->controlId)
                    {
                        proxy->control = c;
                        break;
                    }
                }
                proxy->control->proxy_bak.push_back(proxy);
                break;
            }
        default:
            break;
        }
    }
}

int main()
{
    //解析参数
	// opts = parseArgs()

	//初始化日志功能
	// log.LogTo(opts.logto, opts.loglevel)

    //读配置文件
    if(config.Load("../conf/server.config") == false) //把配置文件内容载入到内存
    {
        printf("配置文件载入失败，退出!\n");
        exit(1);
    }
    // json::string_stream ss;
    // confServer t = *(config.conf);
	// json::to_json(ss, t);
	// std::cout <<"配置json："<< ss.str() << std::endl;
	//开始监听端口，等待客户端链入
    io_context.init();
    std::shared_ptr<Socket> listener = std::shared_ptr<Socket>(new Socket{io_context, config.conf->bindPort});
    controlProxyListener(listener).resume();
    io_context.run();
}