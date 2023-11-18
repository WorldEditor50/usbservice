#ifndef HIDSERVICE_H
#define HIDSERVICE_H
#include <list>
#include "hidcore.h"
#include "pipeline.hpp"
#include "singleton.hpp"


class Buffer : public ByteArray
{
public:
    constexpr static std::size_t max_buffer_size = 1024*1024;
public:
    Buffer()
    {
        used = 0;
        totalSize = 0;
        capacity = max_buffer_size;
        ptr = allocator.allocate(capacity);
    }

    int operator()(std::string &rawpacket, unsigned char* data, int len)
    {
        /* check header 0xAF08DF01 */
        if (data[0] == 0xAF && data[1] == 0x08 &&
                data[2] == 0xDF && data[3] == 0x01) {
            /* parse total size of a packet */
            PacketHead *head = (PacketHead*)data;
            totalSize = head->totalSize;
            if (totalSize > capacity) {
                allocator.free(capacity, ptr);
                capacity = Allocator::align(totalSize);
                ptr = allocator.allocate(capacity);
            }
            /* clear */
            memset(ptr, 0, capacity);
            used = 0;
        }
        /* write */
        memcpy(ptr + used, data, len);
        used += len;
        /* finsihed */
        if (used >= totalSize) {
            rawpacket = std::string((char*)ptr, totalSize);
            return 0;
        }
        return -1;
    }
};

class Pack
{
private:
    std::map<unsigned int, Buffer> buffers;
public:
    int operator()(std::string &rawpacket, unsigned char* data, int len)
    {
        PacketHead *head = (PacketHead*)data;
        unsigned int type = head->messageType;
        if (buffers.find(type) == buffers.end()) {
            buffers.insert(std::pair<unsigned int, Buffer>(type, Buffer()));
        }
        return buffers[type](rawpacket, data, len);
    }
};


class HidService_ : public Hidcore
{
private:
    bool _isConnected;
    Pipeline pipeline;
    Pack pack;
protected:
    void packBuffer(unsigned char* buffer, int len) override;
public:
    HidService_();
    virtual ~HidService_();
    int launch();
    void stop() override;
    void registerRouter(IRouter* router);
    bool isConnected() const {return _isConnected;}
    void setConnected(bool on) {_isConnected = on;}
};

using HidService = Singleton<HidService_>;
#endif // HIDSERVICE_H
