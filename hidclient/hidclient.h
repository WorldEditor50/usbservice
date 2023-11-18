#ifndef HIDCLIENT_H
#define HIDCLIENT_H
#include "../hidkit/hidservice.h"
#include "../hidkit/singleton.hpp"


class HidClient_ : public HidService_
{
public:
    HidClient_();
    void connect();
    void disconnect();
    void sendText(const std::string &text);
};

using HidClient = Singleton<HidClient_>;
#endif // HIDCLIENT_H
