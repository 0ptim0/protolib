#include "stm32_base.h"
#pragma once

#define FEND    0xC0
#define FESC    0xDB
#define TFEND   0xDC
#define TFESC   0xDD

#define BUF_SIZE 128

struct wake_packet_t {
    uint8_t to;
    uint8_t cmd;
    uint8_t length;
    uint8_t *data;
    uint8_t max_data_length;
};

class wake_class {
private:
    void AddToBuf(uint8_t data);
    void AddToPacket(uint8_t data);
    uint8_t TakeFromPacket(uint8_t *data);
    uint8_t CheckCRC(void);
    uint8_t CheckDataLength(uint8_t length, uint8_t max_length);
    uint8_t buf[BUF_SIZE];
    uint8_t buf_length;
    uint8_t ClearBuf(void);
public:
    wake_class() : buf_length(0) {}
    uint8_t Packing(wake_packet_t *packet);
    uint8_t Unpacking(wake_packet_t *packet);
    uint8_t GetBufSize(void);
    uint8_t *GetBufPtr(void);
};
