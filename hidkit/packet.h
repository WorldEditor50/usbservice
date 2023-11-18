#ifndef PACKET_H
#define PACKET_H
#include <string>
#include <vector>
#include <cstring>
#include "bytearray.hpp"

#define PACKET_SIGNATURE 0xAF08DF01
#define PACKET_END 0xAFEF

enum PacketType {
    PACKET_REQ = 0,
    PACKET_RESP
};

struct PacketHead {
    unsigned int signature;
    unsigned int totalSize;
    unsigned int timestamp;
    unsigned int index;
    unsigned int messageType;
    unsigned int packetType;
    unsigned int messageSize;
    unsigned int dataSize;
};

class Packet : public PacketHead
{
public:
    std::string message;
    ByteArray data;
    unsigned short end;
public:
    Packet();
    ~Packet();
    Packet(const Packet &r);
    Packet(Packet &&r);
    Packet& operator=(const Packet &r);
    Packet& operator=(Packet &&r);
    Packet(const unsigned char* rawpacket, int totalSize);
    Packet(const std::string &rawpacket);
    bool isValid() const { return end == PACKET_END;}
    std::string toString();

};

#endif // PACKET_H
