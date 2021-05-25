// #include <vector>
#include "../conf/conf.h"
#include "../frame/io_context.h"
#include "../socket/socket.h"
#include "../frame/task.h"
#include "../channel/channel.h"
#include "control.h"

Channel<int> channel;
std::vector<Control> controlRegistry;
Config<confServer> config;
IOContext io_context;

std::task<> controlListener(std::shared_ptr<Socket> listener)
{
    while (true)
    {
        Control *c = new Control;
        c->socket = co_await listener->accept();
        c->controlCoRoutine().resume();
        controlRegistry.push_back(*c);
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
    // iguana::string_stream ss;
    // confServer t = *(config.conf);
	// iguana::json::to_json(ss, t);
	// std::cout <<"配置json："<< ss.str() << std::endl;
	//开始监听端口，等待客户端链入
    io_context.init();
    std::shared_ptr<Socket> listener = std::shared_ptr<Socket>(new Socket{io_context, config.conf->bindPort});
    controlListener(listener).resume();
    io_context.run();
}