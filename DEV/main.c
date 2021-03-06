//
//  main.c
//  DEV
//
//  Created by Benjamin CHOLET on 12/02/2017.
//

#include "main.h"
#include "cmd.h"
#include "serial.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {
    
    int fd;
    serial_init(&fd); //establish connection
    
    init(fd);

    unsigned char *message = NULL;
    
    buildPacket(&message, 4, readTagMultiple, (unsigned char []){0x00, 0x03, 0x01, 0xE8}); //Timeout (ms)
    serial_write(fd, message);
    serial_read_to_stdout(fd);

    readBuffer(fd);
    
    return 0;
}

void init(int fd)
{
    unsigned char *message = NULL;
    
    buildPacket(&message, 0, bootFirmware, NULL); //start bootloader
    serial_write(fd, message);
    serial_read_to_stdout(fd);
    
    buildPacket(&message, 1, setCurrentRegion, (unsigned char []){0x08}); //EU3
    serial_write(fd, message);
    serial_read_to_stdout(fd);
    
    buildPacket(&message, 2, setCurrentTagProtocol, (unsigned char []){0x00, 0x05}); //Gen2
    serial_write(fd, message);
    serial_read_to_stdout(fd);
    
    buildPacket(&message, 2, setReadTXPower, (unsigned char []){0x09, 0xC4}); //25.0dBm
    serial_write(fd, message);
    serial_read_to_stdout(fd);
    
    buildPacket(&message, 5, setAntennaPort, (unsigned char []){0x02, 0x01, 0x01, 0x01, 0x02}); //altern monostatic - bistatic.
    serial_write(fd, message);
    serial_read_to_stdout(fd);
}
