#include "tunnelClient.h"

extern Config<confClient> config;

void Tunnel::NewTunnel()
{
    for(auto t:(config.conf)->tunnel)
    {
        if(t.remotePort == remotePort)
        {
            localAddr = t.localAddr;
            localPort = t.localPort;
            break;
        }
    }
}