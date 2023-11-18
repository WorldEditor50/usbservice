#include "hidservice.h"

void HidService_::packBuffer(unsigned char* buffer, int len)
{
    std::string rawpacket;
    int ret = pack(rawpacket, buffer, len);
    if (ret != 0) {
        return;
    }
    pipeline.dispatch(rawpacket);
    return;
}

HidService_::HidService_()
{

}

HidService_::~HidService_()
{

}

int HidService_::launch()
{
    pipeline.start();
    return Hidcore::start("/dev/hidraw0");
}

void HidService_::stop()
{
    pipeline.stop();
    return Hidcore::stop();
}

void HidService_::registerRouter(IRouter *router)
{
    pipeline.registerRouter(router);
    return;
}
