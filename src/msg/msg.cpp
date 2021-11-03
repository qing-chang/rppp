#include "msg.h"

namespace _msg_
{
    std::task<> readMsg(std::shared_ptr<Socket> socket, char *rcvBuff, ssize_t *nbRcved, Msg *pmsg)
    {
        ssize_t res;
        msgHdr *h = (msgHdr *)rcvBuff;
        while((*nbRcved) < 4)
        {
            res = co_await socket->recv(rcvBuff + (*nbRcved), RCV_BUFF_SIZE - (*nbRcved));
            (*nbRcved) += res;
        }
        while((*nbRcved) < h->len)
        {
            res = co_await socket->recv(rcvBuff + (*nbRcved), RCV_BUFF_SIZE - (*nbRcved));
            (*nbRcved) += res;
        }
        //---------------------------------------------------------------------------------------------
        //实现得不够优雅，如何改进？
        pmsg->type = (msgType)(h->type);
        switch(pmsg->type)
        {
        case  msgType::Auth :
            pmsg->msg_ = std::shared_ptr<auth>(new auth);
            json::from_json0(*(std::static_pointer_cast<auth>(pmsg->msg_)), 
                                rcvBuff + 4,
                                h->len - 4);
            break;
        case  msgType::AuthResp :
            pmsg->msg_ = std::shared_ptr<authResp>(new authResp);
            json::from_json0(*(std::static_pointer_cast<authResp>(pmsg->msg_)), 
                                rcvBuff + 4,
                                h->len - 4);
            break;
        case  msgType::ReqTunnel :
            pmsg->msg_ = std::shared_ptr<reqTunnel>(new reqTunnel);
            json::from_json0(*(std::static_pointer_cast<reqTunnel>(pmsg->msg_)), 
                                rcvBuff + 4,
                                h->len - 4);
            break;
        case  msgType::NewTunnel :
            pmsg->msg_ = std::shared_ptr<newTunnel>(new newTunnel);
            json::from_json0(*(std::static_pointer_cast<newTunnel>(pmsg->msg_)), 
                                rcvBuff + 4,
                                h->len - 4);
            break;
        case msgType::ReqProxy :
            pmsg->msg_ = std::shared_ptr<reqProxy>(new reqProxy);
            json::from_json0(*(std::static_pointer_cast<reqProxy>(pmsg->msg_)),
                                rcvBuff + 4,
                                h->len - 4);
            break;
        case msgType::RegProxy :
            pmsg->msg_ = std::shared_ptr<regProxy>(new regProxy);
            json::from_json0(*(std::static_pointer_cast<regProxy>(pmsg->msg_)),
                                rcvBuff + 4,
                                h->len - 4);
            break;
        case msgType::StartProxy :
            pmsg->msg_ = std::shared_ptr<startProxy>(new startProxy);
            json::from_json0(*(std::static_pointer_cast<startProxy>(pmsg->msg_)),
                                rcvBuff + 4,
                                h->len - 4);
            break;
        }
        //---------------------------------------------------------------------------------------------
        ssize_t len_ = h->len;
        if((*nbRcved) > len_)
        {
            memcpy(rcvBuff, rcvBuff + len_, (*nbRcved) - len_);
            *nbRcved -= len_;
        } else
        {
            *nbRcved = 0;
        }
    }

    std::task<> writeMsg(std::shared_ptr<Socket> socket, char *sndBuff, Msg *pmsg)
    {
        json::string_stream ss;
        switch(pmsg->type)
        {
        case  msgType::Auth :
            json::to_json(ss, *(std::static_pointer_cast<auth>(pmsg->msg_)));
            break;
        case  msgType::AuthResp :
            json::to_json(ss, *(std::static_pointer_cast<authResp>(pmsg->msg_)));
            break;
        case  msgType::ReqTunnel :
            json::to_json(ss, *(std::static_pointer_cast<reqTunnel>(pmsg->msg_)));
            break;
        case  msgType::NewTunnel :
            json::to_json(ss, *(std::static_pointer_cast<newTunnel>(pmsg->msg_)));
            break;
        case  msgType::ReqProxy :
            json::to_json(ss, *(std::static_pointer_cast<reqProxy>(pmsg->msg_)));
            break;
        case  msgType::RegProxy :
            json::to_json(ss, *(std::static_pointer_cast<regProxy>(pmsg->msg_)));
            break;
        case  msgType::StartProxy :
            json::to_json(ss, *(std::static_pointer_cast<startProxy>(pmsg->msg_)));
            break;
        case msgType::Ping :
        case msgType::Pong :
        default :
            break;
        }
        auto json_str = ss.str();
        json_str.copy(sndBuff + 4, json_str.length(), 0);
        ssize_t len = json_str.length() + 4;
        msgHdr *h = (msgHdr *)sndBuff;
        *h = {pmsg->type, (uint16_t)(len)};
        ssize_t nbSended = 0;
        while (nbSended < len)
        {
            ssize_t res = co_await socket->send(sndBuff + nbSended, len - nbSended);
            if (res <= 0)std::cout << "1--------------------"<<res << std::endl;
                break;
            nbSended += res;
        }
    }
}