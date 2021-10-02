#include "crc.h"


uint8_t crc8(uint8_t *buf, uint8_t length)
{
    uint8_t crc = INIT;
    uint8_t i;
 
    while (length--) {
        crc ^= *buf++;
        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ POLY : crc << 1;
    }
    crc ^= 0xFF;
    return crc;
}