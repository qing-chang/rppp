#pragma once
#include "../socket/socket.h"
#include "../json/json.hpp"

#define BUFFER_SIZE 1024
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
	// Version   string // protocol version
	// MmVersion string // major/minor software version (informational only)
	std::string user;
	std::string password;
	// OS        string
	// Arch      string
	// ClientId  string // empty for new sessions
};
REFLECTION(auth, user, password)

struct authResp
{
	// Version   string
	// MmVersion string
	std::string clientId;
	// Error     string
};
REFLECTION(authResp, clientId)

struct reqTunnel
{
	std::string type;
    std::string name;
	// std::string localAddr;
	// int localPort;
	int remotePort;
};
REFLECTION(reqTunnel, type, name, remotePort)

struct newTunnel
{
	std::string tunnelId;
	// Url      string
	// Protocol string
	// Error    string
};
REFLECTION(newTunnel, tunnelId)

struct reqProxy
{

};

struct regProxy
{
    std::string clientId;
};
REFLECTION(regProxy, clientId)

struct startProxy
{
	// Url        string // URL of the tunnel this connection connection is being proxied for
	// ClientAddr string // Network address of the client initiating the connection to the tunnel
};
// REFLECTION(StartProxy)

struct ping
{
};

struct pong
{
};

class Msg
{
public:
    Msg()
    {}
    ~Msg()
    {}

    msgType type;
    std::shared_ptr<void> msg_;     //std::any?

    static std::task<> readMsg(std::shared_ptr<Socket> socket, char *rcvBuff, ssize_t *nbRcved, Msg *pmsg)
    {
        ssize_t res;
        msgHdr *h = (msgHdr *)rcvBuff;
        while((*nbRcved) < 4)
        {
            res = co_await socket->recv(rcvBuff, RCV_BUFF_SIZE - (*nbRcved));
            (*nbRcved) += res;
        }
        while((*nbRcved) < h->len)
        {
            res = co_await socket->recv(rcvBuff, RCV_BUFF_SIZE - (*nbRcved));
            (*nbRcved) += res;
        }
        pmsg->type = (msgType)(h->type);
        switch(pmsg->type)
        {
        case  msgType::Auth :
            pmsg->msg_ = std::shared_ptr<auth>(new auth);
            iguana::json::from_json0(*(std::static_pointer_cast<auth>(pmsg->msg_)), 
                                        rcvBuff + 4,
                                        h->len - 4);
            break;
        case  msgType::AuthResp :
            pmsg->msg_ = std::shared_ptr<authResp>(new authResp);
            iguana::json::from_json0(*(std::static_pointer_cast<authResp>(pmsg->msg_)), 
                                        rcvBuff + 4,
                                        h->len - 4);
            break;
        case  msgType::ReqTunnel :
            pmsg->msg_ = std::shared_ptr<reqTunnel>(new reqTunnel);
            iguana::json::from_json0(*(std::static_pointer_cast<reqTunnel>(pmsg->msg_)), 
                                        rcvBuff + 4,
                                        h->len - 4);
        case  msgType::NewTunnel :
            pmsg->msg_ = std::shared_ptr<newTunnel>(new newTunnel);
            iguana::json::from_json0(*(std::static_pointer_cast<newTunnel>(pmsg->msg_)), 
                                        rcvBuff + 4,
                                        h->len - 4);
            break;
        case msgType::RegProxy :
            pmsg->msg_ = std::shared_ptr<regProxy>(new regProxy);
            iguana::json::from_json0(*(std::static_pointer_cast<regProxy>(pmsg->msg_)),
                                        rcvBuff + 4,
                                        h->len - 4);
            break;
        }
        if((*nbRcved) != 0)
        {
            ssize_t len_ = h->len;
            if((*nbRcved) > len_)
            {
                *nbRcved -= len_;
                memcpy(rcvBuff, rcvBuff + len_, (*nbRcved) - len_);
            } else
            {
                *nbRcved = 0;
            }
        }
    }

    static std::task<> writeMsg(std::shared_ptr<Socket> socket, char *sndBuff, Msg *pmsg)
    {
        iguana::string_stream ss;
        switch(pmsg->type)
        {
        case  msgType::Auth :
            iguana::json::to_json(ss, *(std::static_pointer_cast<auth>(pmsg->msg_)));
            break;
        case  msgType::AuthResp :
            iguana::json::to_json(ss, *(std::static_pointer_cast<authResp>(pmsg->msg_)));
            break;
        case  msgType::ReqTunnel :
            iguana::json::to_json(ss, *(std::static_pointer_cast<reqTunnel>(pmsg->msg_)));
            break;
        case  msgType::NewTunnel :
            iguana::json::to_json(ss, *(std::static_pointer_cast<newTunnel>(pmsg->msg_)));
            break;
        case msgType::RegProxy :
            break;
        }
        auto json_str = ss.str();
        json_str.copy(sndBuff + 4, json_str.length(), 0);
        ssize_t nbSend = json_str.length() + 4;
        msgHdr *h = (msgHdr *)sndBuff;
        *h = {pmsg->type, (uint16_t)(nbSend)};
        ssize_t nbSended = 0;
        while (nbSended < nbSend)
        {
            ssize_t res = co_await socket->send(sndBuff + nbSended, nbSend - nbSended);
            if (res <= 0)
                break;
            nbSended += res;
        }
    }
};