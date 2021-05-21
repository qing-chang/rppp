
#include "../conf/conf.h"
#include "../frame/io_context.h"
#include "../socket/socket.h"
#include "../frame/task.h"
#include "../channel/channel.h"
#include "controller.h"
#include "model.h"

Config<confClient> config;
IOContext io_context;

int main()
{
    //解析参数
	// opts = parseArgs()

	//初始化日志功能
	// log.LogTo(opts.logto, opts.loglevel)

    //读配置文件
    if(config.Load("../conf/client.config") == false) //把配置文件内容载入到内存
    {
        printf("配置文件载入失败，退出!\n");
        exit(1);
    }

    iguana::string_stream ss;
    confClient t = *(config.conf);
	iguana::json::to_json(ss, t);
    auto json_str = ss.str();
	std::cout <<"配置json："<< json_str << std::endl;

    io_context.init();
    Controller ctl;
    ctl.Run();
    io_context.run();
}