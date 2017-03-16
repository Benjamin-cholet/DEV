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
    serial_init(&fd);
    
    
    unsigned char *message = NULL;
    
   // unsigned char data[20] = {0x03, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x20, 0x10, 0x12, 0x34};
    
   // unsigned char data [10] = {0x03, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x20, 0x10, 0x12, 0x34};

//    buildPacket(&message, 0, bootFirmware, NULL);
//    
//    serial_write(fd, message);
//
//    serial_read_to_stdout(fd);
    
    unsigned char data[1] = {0x08};
    buildPacket(&message, 1, 0x97, data);
    serial_write(fd, message);
    serial_read_to_stdout(fd);
    
    
//    unsigned char *message=NULL;
//    unsigned char data[3] = {5,6,7};
//    
//    buildPacket(&message, 3, firmwareVersion, data);
//    
//    for (int i =0; i<8; i++) {
//        printf("%x ", message[i]);
//    }
//    
    return 0;
}
