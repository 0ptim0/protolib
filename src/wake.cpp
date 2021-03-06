#include "wake.h"
#include "crc.h"

void wake_class::AddToBuf(uint8_t data) 
{
    buf[buf_length] = data;
    buf_length++;
}

void wake_class::AddToPacket(uint8_t data) 
{
    switch(data) {
        case FEND:
            AddToBuf(FESC);
            AddToBuf(TFEND);
            break;
        case FESC:
            AddToBuf(FESC);
            AddToBuf(TFESC);
            break;
        default:
            AddToBuf(data);
            break;
    }
}

uint8_t wake_class::TakeFromPacket(uint8_t *data) 
{
    uint8_t byte;
    if(buf[buf_length] == FESC && buf[buf_length + 1] == TFEND) {
        byte = FEND;
        buf_length = buf_length + 2;
    } else if(buf[buf_length] == FESC && buf[buf_length + 1] == TFESC) {
        byte = FESC;
        buf_length = buf_length + 2;
    } else {
        byte = buf[buf_length];
        buf_length++;
    }
    *data = byte;
    return byte;
}

uint8_t wake_class::Packing(wake_packet_t *packet)
{
    if(packet == nullptr) return ENXIO;
    if(packet->length == 0) return EINVAL;

    buf_length = 0;

    AddToBuf(FEND);
    AddToPacket(packet->to);
    AddToPacket(packet->cmd);
    AddToPacket(packet->length);
    for(int i = 0; i < packet->length; i++) {
        AddToPacket(packet->data[i]);
    }
    AddToPacket(crc8(buf, buf_length));

    return 0;
}

uint8_t wake_class::Unpacking(wake_packet_t *packet)
{
    if(packet == nullptr) return ENXIO;
    if(buf[0] != FEND) return EPROTO;

    buf_length = 0;

    buf_length++;
    TakeFromPacket(&packet->to);
    TakeFromPacket(&packet->cmd);
    TakeFromPacket(&packet->length);

    if(CheckDataLength(packet->length, packet->max_data_length)) {
        ClearBuf();
        return EPROTO;
    }

    for(int i = 0; i < packet->length; i++) {
        TakeFromPacket(&packet->data[i]);
    }

    if(CheckCRC()) {
        ClearBuf();
        return EPROTO;
    }

    ClearBuf();

    return 0;
}

uint8_t wake_class::GetBufSize(void)
{
    return BUF_SIZE - 1;
}

uint8_t *wake_class::GetBufPtr(void)
{
    return buf;
}

uint8_t wake_class::GetBufLength(void)
{
    return buf_length;
}


uint8_t wake_class::CheckCRC(void)
{
    uint8_t crcr, crcc;
    crcc = crc8(buf, buf_length);
    TakeFromPacket(&crcr);
    if(crcc != crcr) {
        return EPROTO;
    }
    return 0;
}

uint8_t wake_class::CheckDataLength(uint8_t length, uint8_t max_data_length)
{
    if(length > max_data_length) {
        return EPROTO;
    }
    return 0;
}

uint8_t wake_class::ClearBuf(void)
{
    for(int i = 0; i < BUF_SIZE; i++) {
        buf[i] = 0;
    }
    return 0;
}
