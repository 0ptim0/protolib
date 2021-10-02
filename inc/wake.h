#include "stm32_base.h"
#pragma once

#define FEND    0xC0
#define FESC    0xDB
#define TFEND   0xDC
#define TFESC   0xDD

struct wake_packet_t {
    uint8_t to;
    uint8_t cmd;
    uint8_t length;
    uint8_t *data;
    uint8_t err;
};

class wake_class {
private:
    void AddToBuf(uint8_t data);
    void AddToPacket(uint8_t data);
    uint8_t TakeFromPacket(uint8_t *data);
    uint8_t CheckCRC(void);
public:
    uint8_t buf[256];
    uint8_t buf_length;
    wake_class() : buf_length(0) {}
    uint8_t Packing(wake_packet_t *packet);
    uint8_t Unpacking(wake_packet_t *packet);
    uint8_t GetBufLength(void);
    uint8_t *GetBufPtr(void);
};
