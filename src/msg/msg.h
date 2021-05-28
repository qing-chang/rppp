#pragma once
#include "../socket/socket.h"
#include "../json/json.hpp"

#define SND_BUFF_SIZE 1024
#define RCV_BUFF_SIZE 1024

struct msgHdr
{
    uint16_t type;
    uint16_t len;
};

enum msgType
{
    Auth       = 1,
    AuthResp   = 2,
    ReqTunnel  = 3,
    NewTunnel  = 4,
    ReqProxy   = 5,
    RegProxy   = 6,
    StartProxy = 7,
    Ping       = 8,
    Pong       = 9
};

struct auth
{
	std::string user;
	std::string password;
};
REFLECTION(auth, user, password)

struct authResp
{
	std::string controlId;
};
REFLECTION(authResp, controlId)

struct reqTunnel
{
	std::string type;
    std::string name;
	int remotePort;
};
REFLECTION(reqTunnel, type, name, remotePort)

struct newTunnel
{
	std::string remotePort;
};
REFLECTION(newTunnel, remotePort)

struct reqProxy
{
    std::string remotePort;
};
REFLECTION(reqProxy, remotePort)

struct regProxy
{
    std::string remotePort;
};
REFLECTION(regProxy, remotePort)

struct startProxy
{
	std::string clientAddr;
};
REFLECTION(startProxy, clientAddr)

struct Msg
{
    msgType type;
    std::shared_ptr<void> msg_;     //std::any?
};

namespace _msg_
{
    std::task<> readMsg(std::shared_ptr<Socket> socket, char *rcvBuff, ssize_t *nbRcved, Msg *pmsg);
    std::task<> writeMsg(std::shared_ptr<Socket> socket, char *sndBuff, Msg *pmsg);
}