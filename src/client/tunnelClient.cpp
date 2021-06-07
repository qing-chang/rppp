#include "tunnelClient.h"

extern Config<confClient> config;

void Tunnel::NewTunnel()
{
    auto tunnelIter = find_if((config.conf)->tunnel.begin(), (config.conf)->tunnel.end(),
        [this](auto t) {
            return (t.remotePort == remotePort);
        });
    if(tunnelIter != (config.conf)->tunnel.end())
    {
        // std::cout <<"查找到匹配的tunnel配置项..."<< std::endl;
        localAddr = (*tunnelIter).localAddr;
        localPort = (*tunnelIter).localPort;
    }
}