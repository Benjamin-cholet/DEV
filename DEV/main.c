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
    
    unsigned char message[5] = {0xFF, 0x00, 0x03, 0x1D, 0x0C};
    int fd;
    
    serial_init(&fd);
    
    serial_write(fd, message);
    
    unsigned char *buf = NULL;
    
    int a;
    
    
    serial_read(fd, &buf, &a);
    
    for (int i=0; i<a; i++) {
        printf("%X ", buf[i]);
    }

    printf("\n%d\n", checkCRC(buf, a));
    
    
    
    return 0;
}
