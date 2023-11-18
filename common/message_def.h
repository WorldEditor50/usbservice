#ifndef MESSAGE_DEF_H
#define MESSAGE_DEF_H


enum MessageType {
    MESSAGE_NONE                    = 0x0000,
    MESSAGE_CONNECT                 = 0x1001,
    MESSAGE_DISCONNECT              = 0x1002,
    MESSAGE_TRANSFER_TEXT           = 0x2001,
    MESSAGE_TRANSFER_IMAGE          = 0x2002,
    MESSAGE_TRANSFER_VIDEO          = 0x2003,
    MESSAGE_TRANSFER_FILE           = 0x2004,
    MESSAGE_OP_LIST_DIR             = 0x3001,
    MESSAGE_OP_RM_FILE              = 0x3002,
    MESSAGE_OP_OPEN_FILE            = 0x3003,
    MESSAGE_STREAM_SCREEN_START     = 0x4001,
    MESSAGE_STREAM_SCREEN_STOP      = 0x4002,
    MESSAGE_STREAM_CAMERA_START     = 0x4003,
    MESSAGE_STREAM_CAMERA_STOP      = 0x4004,
};

#endif // MESSAGE_DEF_H
