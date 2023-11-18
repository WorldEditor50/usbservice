#include "response.h"
#include "../hidkit/utils.h"
#include "../hidkit/packet.h"
#include "../hidkit/bytearray.hpp"
#include "../json/json.h"
#include "../common/message_def.h"

std::string Response::build(int messageType, const std::string &message, std::shared_ptr<unsigned char[]> data, size_t datasize)
{
    Packet packet;
    packet.signature = PACKET_SIGNATURE;
    packet.totalSize = sizeof(PacketHead) + message.size() + datasize;
    packet.timestamp = utils::Clock::timestamp();
    packet.index = 0;
    packet.messageType = messageType;
    packet.packetType = PACKET_RESP;
    packet.messageSize = message.size();
    packet.dataSize = datasize;
    packet.message = message;
    packet.data = ByteArray((unsigned char*)data.get(), datasize);
    packet.end = PACKET_END;
    return packet.toString();
}

std::string Response::build(int messageType, const std::string &message, const std::string &data)
{
    Packet packet;
    packet.signature = PACKET_SIGNATURE;
    packet.totalSize = sizeof(PacketHead) + message.size() + data.size();
    packet.timestamp = utils::Clock::timestamp();
    packet.index = 0;
    packet.messageType = messageType;
    packet.packetType = PACKET_RESP;
    packet.messageSize = message.size();
    packet.dataSize = data.size();
    packet.message = message;
    packet.data = ByteArray::fromString(data);
    packet.end = PACKET_END;
    return packet.toString();
}

std::string Response::build(int messageType, const std::string &message)
{
    Packet packet;
    packet.signature = PACKET_SIGNATURE;
    packet.totalSize = sizeof(PacketHead) + message.size();
    packet.timestamp = utils::Clock::timestamp();
    packet.index = 0;
    packet.messageType = messageType;
    packet.packetType = PACKET_RESP;
    packet.messageSize = message.size();
    packet.dataSize = 0;
    packet.message = message;
    packet.end = PACKET_END;
    return packet.toString();
}

std::string Response::ok(int messageType)
{
    /*
        {"type":message type, "code":0, "result":"ok"}
    */
    Json::Value root;
    root["type"] = messageType;
    root["code"] = 0;
    root["result"] = "ok";
    return build(messageType, root.toStyledString());
}

std::string Response::text(const std::string &content)
{
    /*
        {"type":message type, "code":0, "result":"ok"}
    */
    Json::Value root;
    root["type"] = MESSAGE_TRANSFER_TEXT;
    root["code"] = 0;
    root["result"] = content;
    return build(MESSAGE_TRANSFER_TEXT, root.toStyledString());
}
