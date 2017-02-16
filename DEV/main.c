//
//  main.c
//  DEV
//
//  Created by Benjamin on 12/02/2017.
//  Copyright © 2017 Benjamin. All rights reserved.
//

#include "main.h"
#include "serial.h"

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>


#include <termios.h>

#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/select.h>

int main(int argc, const char * argv[]) {
    
    unsigned char message[5] = {0xFF, 0x00, 0x03, 0x1D, 0x0C};
    int ret=0;
    int fd;
    
    serial_init(&fd);
    
    serial_write(&fd, message);
    
    unsigned char **buf = NULL;
    
    int a;
    
    //serial_read(fd, 27, &a, buf);
    
    serial_read(&fd, buf, &a);
    
    
    
    for (int i=0; i<a; i++) {
        printf("%x", *buf[i]);

    }

    printf("%d", a);
    
    return 0;
}

//
//
//void serial_read(int fd, int length, int *messageLength, unsigned char *message)
//{
//    struct timeval tv;
//    fd_set set;
//    __DARWIN_FD_ZERO(&set);
//    __DARWIN_FD_SET(fd, &set);
//    tv.tv_sec = 500;
//    tv.tv_usec = 100;
//    *messageLength = 0;
//    int statut = 0;
//    
//    int ret = 0;
//    ret = select(fd+1, &set, NULL, NULL, &tv);
//    
//    if (ret < 0) {
//        printf("error select : %d %s", errno, strerror(errno));
//        exit(EXIT_FAILURE);
//    }
//    
//    do {
//        ret = (int)read(fd, message, (ssize_t)length);
//        
//        if (ret < 0) {
//            printf("error reading : %d %s", errno, strerror(errno));
//            exit(EXIT_FAILURE);
//        }
//        
//        if (ret == 0) {
//            printf("nothing read\n");
//            //exit(EXIT_FAILURE);
//            ret = ioctl(fd, TIOCMGET, &statut);
//            if (ret == -1) {
//                printf("%d, %s\n", errno, strerror(errno));
//                exit(EXIT_FAILURE);
//            }
//        }
//        
//        printf("read sucess : %d\n", ret);
//        
//        length -= ret;
//        *messageLength += ret;
//        message += ret;
//        
//    }
//    while (length >0);
//    
//    
//
//}














