#include "packet.h"

Packet::Packet()
{
    signature = PACKET_SIGNATURE;
    totalSize = 0;
    timestamp = 0;
    index = 0;
    messageType = 0;
    packetType = 0;
    messageSize = 0;
    dataSize = 0;
    end = 0;
}

Packet::~Packet()
{

}

Packet::Packet(const Packet &r)
{
    signature = r.signature;
    totalSize = r.totalSize;
    timestamp = r.timestamp;
    index = r.index;
    messageType = r.messageType;
    packetType = r.packetType;
    messageSize = r.messageSize;
    dataSize = r.dataSize;
    message = r.message;
    data = r.data;
    end = r.end;
}

Packet::Packet(Packet &&r)
{
    signature = r.signature;
    totalSize = r.totalSize;
    timestamp = r.timestamp;
    index = r.index;
    messageType = r.messageType;
    packetType = r.packetType;
    messageSize = r.messageSize;
    dataSize = r.dataSize;
    message = std::move(r.message);
    data = std::move(r.data);
    end = r.end;

    r.signature = 0;
    r.totalSize = 0;
    r.timestamp = 0;
    r.index = 0;
    r.messageType = 0;
    r.packetType = 0;
    r.messageSize = 0;
    r.dataSize = 0;
    r.end = 0;
}

Packet &Packet::operator=(const Packet &r)
{
    if (this == &r) {
        return *this;
    }
    signature = r.signature;
    totalSize = r.totalSize;
    timestamp = r.timestamp;
    index = r.index;
    messageType = r.messageType;
    packetType = r.packetType;
    messageSize = r.messageSize;
    dataSize = r.dataSize;
    message = r.message;
    data = r.data;
    end = r.end;
    return *this;
}

Packet &Packet::operator=(Packet &&r)
{
    if (this == &r) {
        return *this;
    }

    signature = r.signature;
    totalSize = r.totalSize;
    timestamp = r.timestamp;
    index = r.index;
    messageType = r.messageType;
    packetType = r.packetType;
    messageSize = r.messageSize;
    dataSize = r.dataSize;
    message = std::move(r.message);
    data = std::move(r.data);
    end = r.end;

    r.signature = 0;
    r.totalSize = 0;
    r.timestamp = 0;
    r.index = 0;
    r.messageType = 0;
    r.packetType = 0;
    r.messageSize = 0;
    r.dataSize = 0;
    r.end = 0;
    return *this;
}

Packet::Packet(const unsigned char* rawpacket, int totalSize)
{
    char* raw = (char*)rawpacket;
    PacketHead *head = static_cast<PacketHead*>(this);
    memcpy(head, raw, sizeof (PacketHead));
    if (messageSize > 0) {
        message = std::string(raw + sizeof (PacketHead), messageSize);
    }
    if (dataSize > 0) {
        data = ByteArray(rawpacket + sizeof (PacketHead) + messageSize, dataSize);
    }
    std::size_t index = totalSize - 3;
    memcpy(&end, raw + index, sizeof (char)*2);
}

Packet::Packet(const std::string &rawpacket)
{
    char* raw = (char*)rawpacket.c_str();
    PacketHead *head = static_cast<PacketHead*>(this);
    memcpy(head, raw, sizeof (PacketHead));
    if (messageSize > 0) {
        message = std::string(raw + sizeof (PacketHead), messageSize);
    }
    if (dataSize > 0) {
        data = ByteArray((unsigned char*)(raw + sizeof (PacketHead) + messageSize), dataSize);
    }
    std::size_t index = sizeof (PacketHead) + messageSize + dataSize;
    memcpy(&end, raw + index, sizeof (char)*2);
}

std::string Packet::toString()
{
    std::string s = std::string((char*)this, sizeof (PacketHead));
    if (messageSize > 0) {
        s += message;
    }
    if (dataSize > 0) {
        s += data.toString();
    }
    s += std::string((char*)&end, sizeof (unsigned short));
    return s;
}

