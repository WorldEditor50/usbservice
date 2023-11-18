#include "hidcore.h"

bool Hidcore::isInited = false;

void Hidcore::recv()
{
    while (1) {
        {
            std::unique_lock<std::mutex> locker(mutex);
            condit.wait(locker, [=]()->bool{
                return state == STATE_PREPEND || state == STATE_OPENED ||
                        state == STATE_RUN || state == STATE_TERMINATE;
            });
            if (state == STATE_TERMINATE) {
                state = STATE_NONE;
                break;
            } else if (state == STATE_PREPEND) {
                int ret = 0;
                if (property.path.empty()) {
                    if (specifiedUsage == false) {
                        ret = openDevice(property.vendorID, property.productID);
                    } else {
                        ret = openDevice(property.vendorID, property.productID, property.usagePage, property.usage);
                    }
                } else {
                    ret = openDevice(property.path);
                }
                if (ret != HID_SUCCESS) {
                    continue;
                } else {
                    state = STATE_RUN;
                    notify(true);
                }
            } else if (state == STATE_CLOSE) {
                closeDevice();
                state = STATE_TERMINATE;
                continue;
            }
        }

        int len = hid_read_timeout(handle, recvBuffer, max_recv_size, timeout);
        if (len < 0) {
            notify(false);
            handle = nullptr;
            state = STATE_PREPEND;
            continue;
        }
        if (len == 0) {
            continue;
        }
        if (process) {
            process(recvBuffer, len);
        } else {
            packBuffer(recvBuffer, len);
        }
    }
    return;
}

Hidcore::Hidcore():
    handle(nullptr),
    state(STATE_NONE),
    timeout(1000),
    specifiedUsage(false),
    recvBuffer(nullptr)
{
    recvBuffer = new unsigned char[max_recv_size];
    notify = [](bool){};
    if (!Hidcore::isInited) {
        hid_init();
        Hidcore::isInited = true;
    }
}

Hidcore::~Hidcore()
{
    if (recvBuffer != nullptr) {
        delete [] recvBuffer;
        recvBuffer = nullptr;
    }
    if (Hidcore::isInited) {
        hid_exit();
        Hidcore::isInited = false;
    }
}

std::vector<Hidcore::Property> Hidcore::enumerate()
{
    std::vector<Hidcore::Property> devices;
    struct hid_device_info *devs = nullptr;
    struct hid_device_info *curDevice = nullptr;

    devs = hid_enumerate(0x0, 0x0);
    curDevice = devs;
    while (curDevice != nullptr) {
        Hidcore::Property dev;
        dev.vendorID = curDevice->vendor_id;
        dev.productID = curDevice->product_id;
        dev.usagePage = curDevice->usage_page;
        dev.usage = curDevice->usage;
        devices.push_back(dev);
        curDevice = curDevice->next;
    }
    hid_free_enumeration(devs);
    return devices;
}

int Hidcore::openDevice(const std::string &path)
{
    if (handle != nullptr) {
        return HID_SUCCESS;
    }
    handle = hid_open_path(path.c_str());
    if (handle == nullptr) {
        return HID_OPEN_FAILED;
    }
    property.path = path;
    return HID_SUCCESS;
}

int Hidcore::openDevice(unsigned short vid, unsigned short pid)
{
    if (handle != nullptr) {
        return HID_SUCCESS;
    }
    handle = hid_open(vid, pid, nullptr);
    if (handle == nullptr) {
        return HID_OPEN_FAILED;
    }
    property.vendorID = vid;
    property.productID = pid;
    return HID_SUCCESS;
}

int Hidcore::openDevice(unsigned short vid, unsigned short pid, unsigned short usagePage, unsigned short usage)
{
    if (handle != nullptr) {
        return HID_SUCCESS;
    }
    struct hid_device_info *devs = nullptr;
    struct hid_device_info *dev = nullptr;
    const char *devPath = nullptr;
    devs = hid_enumerate(vid, pid);
    dev = devs;
    while (dev != nullptr) {
        if (dev->vendor_id == vid && dev->product_id == pid &&
                dev->usage == usage && dev->usage_page == usagePage) {
            devPath = dev->path;
            break;
        }
        dev = dev->next;
    }

    if (devPath != nullptr) {
        /* Open the device */
        handle = hid_open_path(devPath);
    } else {
        hid_free_enumeration(devs);
        return HID_OPEN_FAILED;
    }
    hid_free_enumeration(devs);
    specifiedUsage = true;
    return HID_SUCCESS;
}

void Hidcore::closeDevice()
{
    if (handle != nullptr) {
        hid_close(handle);
    }
    return;
}

void Hidcore::setNonBlock(bool on)
{
    if (handle == nullptr) {
        return;
    }
    hid_set_nonblocking(handle, on);
    return;
}

void Hidcore::setTimeout(int msec)
{
    timeout = msec;
    return;
}

void Hidcore::registerProcess(const Hidcore::FnProcess &func)
{
    process = func;
    return;
}

void Hidcore::registerNotify(const Hidcore::FnNotify &func)
{
    notify = func;
    return;
}

int Hidcore::writeRaw(const unsigned char *data, std::size_t datasize)
{
    if (handle == nullptr) {
        return HID_OPEN_FAILED;
    }
    std::size_t pos = 0; 
#if WIN32
    unsigned char buffer[max_send_size + 1] = {0};
    while (pos < datasize) {
        memset(buffer, 0, max_send_size);
        if (datasize - pos > max_send_size - 1) {
            memcpy(buffer + 1, data + pos, max_send_size - 1);
        } else {
            memcpy(buffer + 1, data + pos, datasize - pos);
        }
        int len = hid_write(handle, buffer, max_send_size);
        if (len < 0) {
            return HID_WRITE_FAILED;
        }
        pos += len + 1;
    }
#else
    unsigned char buffer[max_send_size] = {0};
    while (pos < datasize) {
        memset(buffer, 0, max_send_size);
        if (datasize - pos > max_send_size) {
            memcpy(buffer, data + pos, max_send_size);
        } else {
            memcpy(buffer, data + pos, datasize - pos);
        }
        int len = hid_write(handle, buffer, max_send_size);
        if (len < 0) {
            return HID_WRITE_FAILED;
        }
        pos += len;
    }
#endif
    return HID_SUCCESS;
}

int Hidcore::write(const std::string &data)
{
    return Hidcore::writeRaw((unsigned char*)data.c_str(), data.size());
}

int Hidcore::read(unsigned char *&data, size_t &datasize)
{
    if (handle == nullptr) {
        return HID_OPEN_FAILED;
    }

    int len = hid_read(handle, data, datasize);
    if (len < 0) {
        return HID_READ_FAILED;
    }
    datasize = len;
    return HID_SUCCESS;
}

int Hidcore::sendFeatureReport(const unsigned char *data, size_t datasize)
{
    if (handle == nullptr) {
        return HID_OPEN_FAILED;
    }
    int len = hid_send_feature_report(handle, data, datasize);
    if (len < 0) {
        return HID_SEND_FEATURE_REPORT_FAILED;
    }
    return HID_SUCCESS;
}

int Hidcore::recvFeatureReport(unsigned char *&data, size_t &datasize)
{
    if (handle == nullptr) {
        return HID_OPEN_FAILED;
    }

    int len = hid_get_feature_report(handle, data, datasize);
    if (len < 0) {
        return HID_SEND_FEATURE_REPORT_FAILED;
    }
    datasize = len;
    return HID_SUCCESS;
}

int Hidcore::start(const std::string &path)
{
    if (state != STATE_NONE) {
        return HID_SUCCESS;
    }
    int ret = openDevice(path);
    if (ret != HID_SUCCESS) {
        return ret;
    }
    state = STATE_PREPEND;
    recvThread = std::thread(&Hidcore::recv, this);
    return HID_SUCCESS;
}

int Hidcore::start(unsigned short vid, unsigned short pid)
{
    if (state != STATE_NONE) {
        return HID_SUCCESS;
    }
    int ret = openDevice(vid, pid);
    if (ret != HID_SUCCESS) {
        return ret;
    }
    state = STATE_PREPEND;
    recvThread = std::thread(&Hidcore::recv, this);
    return HID_SUCCESS;
}

int Hidcore::start(unsigned short vid, unsigned short pid, unsigned short usagePage, unsigned short usage)
{
    if (state != STATE_NONE) {
        return HID_SUCCESS;
    }
    int ret = openDevice(vid, pid, usagePage, usage);
    if (ret != HID_SUCCESS) {
        return ret;
    }
    state = STATE_PREPEND;
    recvThread = std::thread(&Hidcore::recv, this);
    return HID_SUCCESS;
}

void Hidcore::stop()
{
    if (state == STATE_NONE) {
        return;
    }

    {
        std::unique_lock<std::mutex> locker(mutex);
        state = STATE_CLOSE;
        condit.notify_all();
    }

    recvThread.join();
    return;
}
