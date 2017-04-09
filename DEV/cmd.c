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
