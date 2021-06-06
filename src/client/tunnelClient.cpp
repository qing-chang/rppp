#include "tunnelClient.h"

extern Config<confClient> config;

void Tunnel::NewTunnel()
{
    // for(auto t:(config.conf)->tunnel)
    // {
    //     if(t.remotePort == remotePort)
    //     {
    //         localAddr = t.localAddr;
    //         localPort = t.localPort;
    //         break;
    //     }
    // }
    // std::string id{std::static_pointer_cast<regProxy>(msg.msg_)->controlId};
    // int remotePort_ = remotePort;
    auto tunnelIter = find_if((config.conf)->tunnel.begin(), (config.conf)->tunnel.end(),
        [this](auto t) {   //confTunnel
            return (t.remotePort == remotePort);
        });
    if(tunnelIter != (config.conf)->tunnel.end())
    {
        // std::cout <<"查找到匹配的tunnel配置项..."<< std::endl;
        localAddr = (*tunnelIter).localAddr;
        localPort = (*tunnelIter).localPort;
    }
}