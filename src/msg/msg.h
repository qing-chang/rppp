#pragma once
#include "../socket/socket.h"
#include "../json/json.hpp"

#define BUFFER_SIZE 1024
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
// REFLECTION(authResp, clientId)

struct reqTunnel
{
	// ReqId    string
	// Protocol string

	// // http only
	// Hostname  string
	// Subdomain string
	// HttpAuth  string

	// // tcp only
	// RemotePort uint16
};
// REFLECTION(reqTunnel)

struct newTunnel
{
	// ReqId    string
	// Url      string
	// Protocol string
	// Error    string
};
// REFLECTION(newTunnel)

struct ReqProxy
{

};

struct RegProxy
{
// ClientId string
};

struct StartProxy
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
    : begin{0},end{0},checked{0},floor{0}
    {}
    ~Msg()
    {}

    char buffer[BUFFER_SIZE];
    int begin;
    int end;
    int checked;
    int floor;

    enum msgType type;

    int checkPack();

    static std::task<> readMsg(std::shared_ptr<Socket> socket, char *rcvBuff, ssize_t *nbRcved, msgHdr *h)
    {
        ssize_t res;
        while((*nbRcved) < 4)
        {
            res = co_await socket->recv(rcvBuff, RCV_BUFF_SIZE - (*nbRcved));
            (*nbRcved) += res;
        }
        h = (msgHdr *)rcvBuff;
        while((*nbRcved) < h->len)
        {
            res = co_await socket->recv(rcvBuff, RCV_BUFF_SIZE - (*nbRcved));
            (*nbRcved) += res;
        }
        if((*nbRcved) > h->len)
        {
            *nbRcved -= h->len;
            memcpy(rcvBuff, rcvBuff + h->len, (*nbRcved) - h->len);
        } else
        {
            *nbRcved = 0;
        }
        // if(begin == 0 && checked == end)
        // {
        //     checked = end = 0;
        // }
        // else
        // {
        //     begin = checked;
        // }
    }

	template <typename T>
    static std::task<> writeMsg(std::shared_ptr<Socket> socket, char *sndBuff, enum msgType type, T *t)
    {
        iguana::string_stream auth_ss;
        iguana::json::to_json(auth_ss, *t);
        auto json_str = auth_ss.str();
        json_str.copy(sndBuff + 4, json_str.length(), 0);
        ssize_t nbSend = json_str.length() + 4;
        msgHdr *h = (msgHdr *)sndBuff;
        *h = {type, (uint16_t)(nbSend)};
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