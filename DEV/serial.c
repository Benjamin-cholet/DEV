//
//  serial.c
//  DEV
//
//  Created by Benjamin on 16/02/2017.
//  Copyright © 2017 Benjamin. All rights reserved.
//

#include "serial.h"
#include "main.h"
#include "cmd.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

static const char *portName = "/dev/cu.usbserial";

void serial_init(int *fd)
{
    struct termios tty;
    int ret;
    
    *fd = open(portName, /*O_NONBLOCK |*/ O_RDWR | O_NOCTTY /*0x20006, 0x5*/);
    if (*fd < 0) {
        printf("error %d openning %s : %s\n", errno, portName, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
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
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    int statut = 0;
    
    *msg = malloc(5 * sizeof(unsigned char));
    *length = 5;
    int i = 5;
    int exited_header = 0;
    
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

void serial_read_to_stdout(int fd)
{
    unsigned char *buf = NULL;
    int a;
    
    serial_read(fd, &buf, &a);
    
    printf("command 0x%.2X ended with code : 0x%.2X%.2X ", buf[2], buf[3], buf[4]);
    
    if (buf[3]*0x100 + buf[4]) {
        M5e_strerror(buf[3]*0x100 + buf[4]);
    }
    
    if (buf[1]!=0) {
        printf("\ndata :");
        for (int i=5; i<(5+buf[1]); i++) {
            printf("%.2X ", buf[i]);
        }
    }
    
    printf("\n%s\n", checkCRC(buf, a) ? "" : "CRC is false");

}
