//
//  cmd.h
//  DEV
//
//  Created by Benjamin on 17/02/2017.
//  Copyright © 2017 Benjamin. All rights reserved.
//

#ifndef cmd_h
#define cmd_h

#include <stdio.h>

typedef enum M5eCmd  {readFlash = 0x02, firmwareVersion = 0x03, bootFirmware = 0x04, setBaudRate = 0x06, verifyImageCRC = 0x08,startBootLoader = 0x09, getCurrentProgram = 0x0C, writeFlashSector = 0x0D, getSectorSize = 0x0E, modifyFlashSector = 0x0F,
    readTagSingle = 0x21, readTagMultiple = 0x22, writeTagEPC = 0x23, writeTagData = 0x24, lockTag = 0x25, killTag = 0x26, readTagData = 0x28, getTagBuffer = 0x29, clearTagBuffer = 0x2A, gen2TagSpecific = 0x2D, blockErase = 0x2E, 
    setAntennaPort = 0x91, setReadTXPower = 0x92, setCurrentTagProtocol = 0x93, setWriteTXPower = 0x94, setFrequencyHopTable =0x95, setUserGPIOOutputs = 0x96, setCurrentRegion = 0x97, setPowerMode = 0x98, setUserMode = 0x99, setReaderConfiguration = 0x9A, setProtocolConfiguration = 0x9B,
getHardwareVersion = 0x10, getAntennaConfiguration = 0x61, getReadTXPower = 0x62, getCurrentTagProtocol = 0x63, getWriteTXPower = 0x64, getFrequencyHopTable = 0x65, getUserGPIOInputs = 0x66, getCurrentRegion = 0x67, getPowerMode = 0x68, getUserMode = 0x69, getReaderConfiguration = 0x6A, getProtocolConfiguration = 0x6B, getReaderStatistics = 0x6C, getAvailableProtocols = 0x70, getAvailableRegions = 0x71, getCurrentTemperature = 0x72
}M5eCmd;

void CRC16_CCITT(uint16_t *crcReg, unsigned char data);
int checkCRC(unsigned char *msg, int lenth);
void buildPacket(unsigned char **msg, unsigned char dataLength, M5eCmd op_code, unsigned char *data);
void M5e_init(int fd);
void M5e_strerror(uint16_t a);


#endif /* cmd_h */
