#pragma once

#include <vector>
#include <string>
#include <string_view>
#include "../json/json.hpp"

struct confServer{
	int bindPort;
    std::unordered_map<std::string, std::string> user;
};
REFLECTION(confServer, bindPort, user)

struct confTunnel{
	std::string type;
    std::string name;
	std::string localAddr;
	int localPort;
	int remotePort;
};
REFLECTION(confTunnel, type, name, localAddr, localPort, remotePort)

struct confClient{
	std::string serverAddr;
	int serverPort;
	std::list<confTunnel> tunnel;
};
REFLECTION(confClient, serverAddr, serverPort, tunnel)

template<typename T>
class Config
{
public:
	Config(){conf = new T;};
	~Config(){delete conf;};
    bool Load(const char *pconfName); //装载配置文件
	T *conf;
};

//装载配置文件
template<typename T>
bool Config<T>::Load(const char *pconfName) 
{
    FILE *fp = nullptr;
    fp = fopen(pconfName,"r");
    if(fp == NULL)
        return false;

    std::string confStr{};
    char linebuf[201];   //每一行配置文件读出来都放这里,每行配置都不要太长，保持<200字符内，防止出现问题

    while(!feof(fp))  //检查文件是否结束 ，没有结束则条件成立
    {
        if(fgets(linebuf,200,fp) == NULL) //从文件中读数据，每次读一行，一行最多不要超过200个字符 
            continue;

        if(linebuf[0] == 0)
            continue;

        //处理注释行
        if(*linebuf=='#')
			continue;

        confStr += linebuf;
    }
    fclose(fp);
    fp = nullptr;
    iguana::json::from_json0(*conf, confStr.data(), confStr.length());
    return true;
}