HEADERS += \
    # libusb
    $$PWD/hidapi/linux/hidapi.h \
    $$PWD/hidapi/linux/hidapi_libusb.h \
    $$PWD/hidapi/linux/hidapi_thread_pthread.h \
    $$PWD/libusb/linux/config.h \
    $$PWD/libusb/linux/events_posix.h \
    $$PWD/libusb/linux/libusb.h \
    $$PWD/libusb/linux/libusbi.h \
    $$PWD/libusb/linux/linux_usbfs.h \
    $$PWD/libusb/linux/threads_posix.h \
    $$PWD/libusb/linux/version.h \
    $$PWD/libusb/linux/version_nano.h \
    # hidapi
    $$PWD/hidapi/hidapi.h \
    $$PWD/hidapi/hidapi_libusb.h \
    $$PWD/hidapi/hidapi_thread_pthread.h \
    # hid
    $$PWD/allocator.hpp \
    $$PWD/asyncqueue.hpp \
    $$PWD/bytearray.hpp \
    $$PWD/configure.hpp \
    $$PWD/hidcore.h \
    $$PWD/hidservice.h \
    $$PWD/irouter.hpp \
    $$PWD/packet.h \
    $$PWD/pipeline.hpp \
    $$PWD/singleton.hpp \
    $$PWD/utils.h

SOURCES += \
    # hidapi
    $$PWD/hidapi/linux/hid.c \
    # libusb
    $$PWD/libusb/linux/core.c \
    $$PWD/libusb/linux/descriptor.c \
    $$PWD/libusb/linux/events_posix.c \
    $$PWD/libusb/linux/hotplug.c \
    $$PWD/libusb/linux/io.c \
    $$PWD/libusb/linux/linux_netlink.c \
    # $$PWD/libusb/linux/linux_udev.c \
    $$PWD/libusb/linux/linux_usbfs.c \
    # $$PWD/libusb/linux/null_usb.c \
    $$PWD/libusb/linux/strerror.c \
    $$PWD/libusb/linux/sync.c \
    $$PWD/libusb/linux/threads_posix.c \
    # hid
    $$PWD/bytearray.cpp \
    $$PWD/hidcore.cpp \
    $$PWD/hidservice.cpp \
    $$PWD/packet.cpp \
    $$PWD/utils.cpp



	
