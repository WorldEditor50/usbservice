#ifndef ROUTER_HPP
#define ROUTER_HPP
#include <map>
#include <string>
#include <memory>
#include "../hidkit/packet.h"
#include "../hidkit/irouter.hpp"
#include "../hidkit/message_def.h"
#include "handler.h"

class Router : public IRouter
{
public:
    using FnHandler = typename Handler::FnHandler;
private:
    std::map<unsigned int, FnHandler> mapper;
public:
    Router()
    {
        mapper.insert(std::pair<unsigned int, FnHandler>(MESSAGE_CONNECT, &Handler::connect));
        mapper.insert(std::pair<unsigned int, FnHandler>(MESSAGE_DISCONNECT, &Handler::disconnect));
        mapper.insert(std::pair<unsigned int, FnHandler>(MESSAGE_TRANSFER_TEXT, &Handler::transferText));
    }

    inline int invoke(const std::string &rawpacket) override
    {
        /* decode */
        Packet packet(rawpacket);
        if (!packet.isValid()) {
            return -1;
        }
        auto it = mapper.find(packet.messageType);
        if (it == mapper.end()) {
            return -2;
        }
        return it->second(packet);
    }

};
#endif // ROUTER_HPP
