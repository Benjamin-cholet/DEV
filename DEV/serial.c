//
//  serial.c
//  DEV
//
//  Created by Benjamin on 16/02/2017.
//  Copyright © 2017 Benjamin. All rights reserved.
//

#include "serial.h"
#include "main.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

void serial_init(int *fd)
{
    struct termios tty;
    int ret;
    
    PRINTD("init...\n");
    
    *fd = open(portName, O_NONBLOCK | O_RDWR | O_NOCTTY /*0x20006, 0x5*/);
    if (fd < 0) {
        printf("error %d openning %s : %s", errno, portName, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    PRINTD("port openned\n");
    
    ret = tcgetattr(*fd, &tty);
    
    if (ret == -1) {
        printf("error tcgetattr : %d, %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    tty.c_iflag &= ~(ICRNL | IGNCR | INLCR | INPCK | ISTRIP | IXANY
                     | IXON | IXOFF | PARMRK);
    tty.c_oflag &= ~OPOST;
    tty.c_cflag &= ~(CRTSCTS | CSIZE | CSTOPB | PARENB);
    tty.c_cflag |= CS8 | CLOCAL | CREAD | HUPCL;
    tty.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1;
    
    cfsetispeed(&tty, 9600);
    cfsetospeed(&tty, 9600);
    
    ret = tcsetattr(*fd, TCSANOW, &tty);
    
    if (ret == -1 ) {
        printf("error tcsetattr %d %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void serial_write(int fd, unsigned char *msg)
{
    PRINTD("writing...\n");
    
    for (int i=0; i<25; i++) {
        printf("%x ", msg[i]);
    }
    
    int length = 5 + (int)msg[1]; //cf protocol : header (1) + dataLength (1) + command (1) + data (n) + CRC (2)
    long ret;
    
    do {
        ret = write(fd, msg, length);
        
        if (ret == -1) {
            printf("error writing %d : %s", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        length -= ret;
        msg += ret;
    } while (length > 0);
}

void serial_read(int fd, unsigned char **msg, int *length)
{
    struct timeval tv;
    fd_set set;
    FD_ZERO(&set);
    FD_SET(fd, &set);
    tv.tv_sec = 500;
    tv.tv_usec = 100;
    int statut = 0;
    
    *msg = malloc(5 * sizeof(unsigned char));
    *length = 5;
    int i = 5;
    int exited_header = 0;

    PRINTD("reading...\n");
    
    int ret = select(fd+1, &set, NULL, NULL, &tv);

    if (ret < 0) {
        printf("error select : %d %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    do {
        ret = (int)read(fd,*msg, (ssize_t)i);

        if (ret < 0) {
            printf("error reading : %d %s", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (ret == 0) {
            //printf("nothing read\n");
            //exit(EXIT_FAILURE);
            ret = ioctl(fd, TIOCMGET, &statut);
            if (ret == -1) {
                printf("%d, %s\n", errno, strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
        
        i -= ret;
        *msg += ret;
        
        if (i==0 && !exited_header) {
            *length += *(*msg - 4) + 2; //add data + CRC
            i += *(*msg - 4) + 2;
            exited_header = 1;
            *msg -= 5;
            *msg = realloc(*msg, *length);
            *msg += 5;
        }
    }
    while (i > 0);
    
    *msg -= *length;
}
