#ifndef HANDLER_H
#define HANDLER_H
#include <string>
#include <functional>
#include "../hidkit/packet.h"


class Handler
{
public:
    using FnHandler = std::function<int(const Packet &packet)>;
public:
    static int connect(const Packet &packet);
    static int disconnect(const Packet &packet);
    static int transferFile(const Packet &packet);
    static int transferText(const Packet &packet);
};


#endif // HANDLER_H
