#ifndef HIDCORE_H
#define HIDCORE_H
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <cstring>
#include "hidapi/win/hidapi.h"


class Hidcore
{
public:
    using FnProcess = std::function<void(unsigned char*, std::size_t)>;
    using FnNotify = std::function<void(bool)>;
    enum State {
        STATE_NONE = 0,
        STATE_PREPEND,
        STATE_OPENED,
        STATE_RUN,
        STATE_PAUSE,
        STATE_CLOSE,
        STATE_TERMINATE
    };
    enum Code {
        HID_SUCCESS = 0,
        HID_OPEN_FAILED,
        HID_WRITE_FAILED,
        HID_READ_FAILED,
        HID_SEND_FEATURE_REPORT_FAILED,
        HID_RECV_FEATURE_REPORT_FAILED
    };

    struct Property {
        unsigned short vendorID;
        unsigned short productID;
        unsigned short usagePage;
        unsigned short usage;
        std::string path;
    };

    constexpr static std::size_t max_recv_size = 1024*1024;
    constexpr static std::size_t max_send_size = 1024;
    static bool isInited;
protected:
    Property property;
    hid_device *handle;
    std::thread recvThread;
    std::mutex mutex;
    std::condition_variable condit;
    FnProcess process;
    FnNotify notify;
    int state;
    int timeout;
    bool specifiedUsage;
    unsigned char* recvBuffer;
protected:
    void recv();
    virtual void packBuffer(unsigned char*, int){}
public:
    Hidcore();
    virtual ~Hidcore();
    static std::vector<Hidcore::Property> enumerate();
    int openDevice(const std::string &path);
    int openDevice(unsigned short vid, unsigned short pid);
    int openDevice(unsigned short vid, unsigned short pid, unsigned short usagePage, unsigned short usage);
    void closeDevice();
    bool isOpened() const {return state == STATE_RUN;}
    void setNonBlock(bool on);
    void setTimeout(int msec);
    void registerProcess(const FnProcess &func);
    void registerNotify(const FnNotify &func);
    int writeRaw(const unsigned char *data, std::size_t datasize);
    int write(const std::string &data);
    int read(unsigned char* &data, std::size_t & datasize);
    int sendFeatureReport(const unsigned char* data, std::size_t datasize);
    int recvFeatureReport(unsigned char* &data, std::size_t &datasize);
    int start(const std::string &path);
    int start(unsigned short vid, unsigned short pid);
    int start(unsigned short vid, unsigned short pid, unsigned short usagePage, unsigned short usage);
    virtual void stop();

};

#endif // HIDCORE_H
