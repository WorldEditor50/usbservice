HEADERS += \
    # hidapi
    $$PWD/hidapi/win/hidapi_cfgmgr32.h \
    $$PWD/hidapi/win/hidapi_descriptor_reconstruct.h \
    $$PWD/hidapi/win/hidapi_hidclass.h \
    $$PWD/hidapi/win/hidapi_hidpi.h \
    $$PWD/hidapi/win/hidapi_hidsdi.h \
    $$PWD/hidapi/win/hidapi_winapi.h \
    $$PWD/hidapi/win/hidapi.h \
    # libusb
    $$PWD/libusb/win/config.h \
    $$PWD/libusb/win/events_windows.h \
    $$PWD/libusb/win/libusb.h \
    $$PWD/libusb/win/libusbi.h \
    $$PWD/libusb/win/threads_windows.h \
    $$PWD/libusb/win/version.h \
    $$PWD/libusb/win/version_nano.h \
    $$PWD/libusb/win/windows_common.h \
    $$PWD/libusb/win/windows_usbdk.h \
    $$PWD/libusb/win/windows_winusb.h \
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
    $$PWD/hidapi/win/hid.c \
    $$PWD/hidapi/win/hidapi_descriptor_reconstruct.c \
    # libusb
    $$PWD/libusb/win/core.c \
    $$PWD/libusb/win/descriptor.c \
    $$PWD/libusb/win/events_windows.c \
    $$PWD/libusb/win/hotplug.c \
    $$PWD/libusb/win/io.c \
    $$PWD/libusb/win/strerror.c \
    $$PWD/libusb/win/sync.c \
    $$PWD/libusb/win/threads_windows.c \
    $$PWD/libusb/win/windows_common.c \
    $$PWD/libusb/win/windows_usbdk.c \
    $$PWD/libusb/win/windows_winusb.c \
    # hid
    $$PWD/bytearray.cpp \
    $$PWD/hidcore.cpp \
    $$PWD/hidservice.cpp \
    $$PWD/packet.cpp \
    $$PWD/utils.cpp \
