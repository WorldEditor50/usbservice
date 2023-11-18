#include "requsest.h"
#include "../hidkit/utils.h"
#include "../json/json.h"
#include "../common/message_def.h"
#include "../hidkit/packet.h"

Requsest::Requsest()
{

}

std::string Requsest::build(int messageType)
{
    Packet packet;
    packet.signature = PACKET_SIGNATURE;
    packet.totalSize = sizeof(PacketHead);
    packet.timestamp = utils::Clock::timestamp();
    packet.index = 0;
    packet.messageType = messageType;
    packet.packetType = PACKET_REQ;
    packet.messageSize = 0;
    packet.dataSize = 0;
    packet.end = PACKET_END;
    return packet.toString();
}

std::string Requsest::build(int messageType, const std::string &message)
{
    Packet packet;
    packet.signature = PACKET_SIGNATURE;
    packet.totalSize = sizeof(PacketHead) + message.size();
    packet.timestamp = utils::Clock::timestamp();
    packet.index = 0;
    packet.messageType = messageType;
    packet.packetType = PACKET_REQ;
    packet.messageSize = message.size();
    packet.dataSize = 0;
    packet.message = message;
    packet.end = PACKET_END;
    return packet.toString();
}

std::string Requsest::build(int messageType, const std::string &message, const std::string &data)
{
    Packet packet;
    packet.signature = PACKET_SIGNATURE;
    packet.totalSize = sizeof(PacketHead) + message.size() + data.size();
    packet.timestamp = utils::Clock::timestamp();
    packet.index = 0;
    packet.messageType = messageType;
    packet.packetType = PACKET_REQ;
    packet.messageSize = message.size();
    packet.dataSize = data.size();
    packet.message = message;
    packet.data = ByteArray::fromString(data);
    packet.end = PACKET_END;
    return packet.toString();
}

std::string Requsest::connect()
{
    return build(MESSAGE_CONNECT);
}

std::string Requsest::disconnect()
{
    return build(MESSAGE_DISCONNECT);
}

std::string Requsest::text(const std::string &content)
{
    return build(MESSAGE_TRANSFER_TEXT, content);
}
