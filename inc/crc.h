#include "stm32_base.h"

#define POLY 0x99
#define INIT 0xDE

uint8_t crc8(uint8_t *data, uint8_t length);