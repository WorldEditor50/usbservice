#include "handler.h"
#include "response.h"
#include "../common/transmitter.h"
#include "../common/message_def.h"
#include "../hidkit/hidservice.h"

int Handler::connect(const Packet &packet)
{
    Transmitter::instance().sendText("service connected.");
    HidService::instance()->setConnected(true);
    HidService::instance()->write(Response::ok(MESSAGE_CONNECT));
    return 0;
}

int Handler::disconnect(const Packet &packet)
{
    Transmitter::instance().sendText("service disconnected.");
    HidService::instance()->setConnected(false);
    HidService::instance()->write(Response::ok(MESSAGE_DISCONNECT));
    return 0;
}

int Handler::transferFile(const Packet &packet)
{

    /*
        message: {"fileName":"", "frameSize":1024, "offset":0, "totalSize":65532}
        data: xxxx
    */
    /* open file */
    while (1) {
        /* read file */


        /* send data */


    }
    return 0;
}

int Handler::transferText(const Packet &packet)
{
    Transmitter::instance().sendText(QString::fromStdString(packet.message));
    HidService::instance()->write(Response::text(packet.message));
    return 0;
}
