//
//  main.c
//  DEV
//
//  Created by Benjamin on 12/02/2017.
//  Copyright © 2017 Benjamin. All rights reserved.
//

#include "main.h"
#include "cmd.h"
#include "serial.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {
    
    int fd;
    serial_init(&fd); //establish connection
    
    
    unsigned char *message = NULL;
    
//    buildPacket(&message, 0, bootFirmware, NULL); //start bootloader
//    serial_write(fd, message);
//    serial_read_to_stdout(fd);
    
//    unsigned char data[1] = {0x08}; //EU3
//    buildPacket(&message, 1, setCurrentRegion, data);
//    serial_write(fd, message);
//    serial_read_to_stdout(fd);
    
//    unsigned char data[2] = {0x00, 0x05}; //Gen2
//    buildPacket(&message, 2, setCurrentTagProtocol, data);
//    serial_write(fd, message);
//    serial_read_to_stdout(fd);
    
//    unsigned char data[2] = {0x09, 0xC4}; //25.0dBm
//    buildPacket(&message, 2, setReadTXPower, data);
//    serial_write(fd, message);
//    serial_read_to_stdout(fd);

//    unsigned char data[2] = {0x01, 0x01}; //TX,RX on port1
//    buildPacket(&message, 2, setAntennaPort, data);
//    serial_write(fd, message);
//    serial_read_to_stdout(fd);
    
    unsigned char data[2] = {0x27, 0x10}; //timeout (ms)
    buildPacket(&message, 2, readTagSingle, data);
    serial_write(fd, message);
    serial_read_to_stdout(fd);
    
    

    return 0;
}
