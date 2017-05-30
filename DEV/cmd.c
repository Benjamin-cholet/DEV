//
//  cmd.c
//  DEV
//
//  Created by Benjamin on 17/02/2017.
//  Copyright © 2017 Benjamin. All rights reserved.
//

#include "cmd.h"
#include "serial.h"

#include <stdlib.h>
#include <sys/types.h>
#include <inttypes.h>
#include <string.h>

struct tag tagBuf[MAXTAG];

void CRC16_CCITT(uint16_t *crcReg, unsigned char data)
{
    uint16_t i;
    uint16_t xorFlag;
    uint16_t bit;
    uint16_t dcdBitMask = 0x80;
    uint16_t poly = 0x1021;
    
    for(i=0; i<8; i++)
    {
        // Get the carry bit. This determines if the polynomial should be // xor'd with the CRC register.
        xorFlag = *crcReg & 0x8000;
        // Shift the bits over by one.
        *crcReg <<= 1;
        // Shift in the next bit in the data byte
        bit = ((data & dcdBitMask) == dcdBitMask);
        *crcReg |= bit;
        // XOR the polynomial
        if(xorFlag)
        {
            *crcReg = *crcReg ^ poly;
        }
        // Shift over the dcd mask
        dcdBitMask >>= 1;
    }
}

int checkCRC(unsigned char *msg, int lenth)
{
    uint16_t crcReg = 0xFFFF;
    
    for (int i=1; i<lenth-2; i++)
        CRC16_CCITT(&crcReg, msg[i]);
    
    if ((msg[lenth-2] == crcReg / 0x100) && (msg[lenth-1] == crcReg % 0x100))
        return 1;
    
    return 0;
}

void buildPacket(unsigned char **msg, unsigned char dataLength, M5eCmd op_code, unsigned char *data)
{
    *msg = malloc((4+dataLength) * sizeof(unsigned char));
    
    **msg = 0xFF;
    *(*msg+1) = dataLength;
    *(*msg+2) = op_code;
    for (int i=3; i<dataLength+3; i++)
    {
        *(*msg+i) = *(data+i-3);
    }
    
    uint16_t crcReg = 0xFFFF;
    for (int i=1; i<dataLength+3; i++)
        CRC16_CCITT(&crcReg, *(*msg+i));
    
    *(*msg + dataLength+3) = crcReg / 0x100;
    *(*msg + dataLength+4) = crcReg % 0x100;
}

void M5e_strerror(uint16_t a){
    switch (a) {
        case 0x101:
            printf("FAULT_INVALID_OPCODE");
            break;
        case 0x105:
            printf("FAULT_MSG_INVALID_PARAMETER_VALUE");
            break;
        case 0x400:
            printf("FAULT_NO_TAGS_FOUND");
            break;
        case 0x402:
            printf("FAULT_INVALID_PROTOCOL_SPECIFIED");
            break;
        case 0x600:
            printf("FAULT_TAG_ID_BUFFER_NOT_ENOUGH_TAGS_AVAILABLE");
            break;
        default:
            printf("unreferenced error");
            break;
    }
}

void clearTagBuf()
{
    for (int i=0; i<MAXTAG; i++) {
        memset(tagBuf[i].EPC, 0x00, 16);
        tagBuf[i].RSSI_11=0;
        tagBuf[i].RSSI_12=0;
    }
}

void tagBufDisp()
{
    int i=0;
    while (i<MAXTAG && tagBuf[i].EPC[0] != '\0') {
        printf("tag #");
        for (int j=0; j<16; j++) {
            printf("%.2X", tagBuf[i].EPC[j]);
        }
        printf(" RSSI11 : %d RSSI12 : %d ∑/∆ = %f\n", tagBuf[i].RSSI_11, tagBuf[i].RSSI_12, (float)tagBuf[i].RSSI_11/tagBuf[i].RSSI_12);
        i++;
    }
}

void readBuffer(int fd)
{
    clearTagBuf();
    unsigned char *message = NULL;
    int a;
    
    buildPacket(&message, 3, getTagBuffer, (unsigned char []){0x00, 0x06, 0x00});
    serial_write(fd, message);
    serial_read(fd, &message, &a);
    
    int tag_count = message[8];
    
    for (int i=0; i < tag_count; i++) {
        unsigned char EPC[16];
        memcpy(EPC, &message[13+i*20],16);
        
        int j=0;
        while (j<MAXTAG) {
            if (tagBuf[j].EPC[0] == 0x00) {
                memcpy(tagBuf[j].EPC, EPC ,16);
                break;
            }
            if (memcmp(EPC, tagBuf[j].EPC, 16) == 0) {
                break;
            }
            j++;
        }
        
        if (message[i*20+10] == 0x11) {
            tagBuf[j].RSSI_11 = message[i*20+9];
        }
        else if (message[i*20+10] == 0x12) {
            tagBuf[j].RSSI_12 = message[i*20+9];
        }
        
    }
    tagBufDisp();
}
