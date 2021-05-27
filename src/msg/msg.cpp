#include "msg.h"

namespace _msg_
{
    std::task<> readMsg(std::shared_ptr<Socket> socket, char *rcvBuff, ssize_t *nbRcved, Msg *pmsg)
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

    std::task<> writeMsg(std::shared_ptr<Socket> socket, char *sndBuff, Msg *pmsg)
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
        case  msgType::ReqProxy :
            iguana::json::to_json(ss, *(std::static_pointer_cast<reqProxy>(pmsg->msg_)));
            break;
        case msgType::Ping :
        case msgType::Pong :
        default :
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
}