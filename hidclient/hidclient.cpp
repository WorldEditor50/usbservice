#include "hidclient.h"
#include "requsest.h"

HidClient_::HidClient_()
{

}

void HidClient_::connect()
{
    Hidcore::write(Requsest::connect());
    return;
}

void HidClient_::disconnect()
{
    Hidcore::write(Requsest::disconnect());
    return;
}

void HidClient_::sendText(const std::string &text)
{
    Hidcore::write(Requsest::text(text));
    return;
}
