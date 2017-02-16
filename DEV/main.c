//
//  main.c
//  DEV
//
//  Created by Benjamin on 12/02/2017.
//  Copyright © 2017 Benjamin. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>


#include <termios.h>

#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/select.h>


int set_interface_attribs (int fd, int speed, int parity);
void set_blocking (int fd, int should_block);

//int main(int argc, const char * argv[]) {
//    
//    char *portname = "/dev/tty.usbserial";
//    
//    int fd = open (portname, O_RDWR);
//    if (fd < 0)
//    {
//        printf ("error %d opening %s: %s", errno, portname, strerror (errno));
//        return 0;
//    }
//    printf("oppenned");
//    set_interface_attribs (fd, B9600, 0);
//    printf("set");// set speed to 115,200 bps, 8n1 (no parity)
//    set_blocking (fd, 0);                // set no blocking
//    printf("blocked");
//    
//    unsigned char buf [100];
//    buf[0] = 0xFF;
//    buf[1] = 0x00;
//    buf[2] = 0x03;
//    buf[3] = 0x1D;
//    buf[4] = 0x0C;
//
//    
//    write (fd, buf, 5);           // send 7 character greeting
//    
//    usleep ((5 + 25) * 1000);             // sleep enough to transmit the 7 plus
//    // receive 25:  approx 100 uS per char transmit
//
//    memset(buf, 0, 7);
//    
//    int n = read (fd, buf, sizeof buf);
//    printf ("error %d reading %s", errno, strerror (errno));
//
//    printf("YES : %s", buf);
//    
//    
//    return 0;
//}
//
//int set_interface_attribs (int fd, int speed, int parity)
//{
//    struct termios tty;
//    memset (&tty, 0, sizeof(tty));
//    if (tcgetattr (fd, &tty) != 0)
//    {
//        printf ("error %d from tcgetattr", errno);
//        return -1;
//    }
//    
//    cfsetospeed (&tty, speed);
//    cfsetispeed (&tty, speed);
//    
//    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
//    tty.c_cflag &= ~CSIZE;
//    tty.c_cflag |= CS8;         /* 8-bit characters */
//    tty.c_cflag &= ~PARENB;     /* no parity bit */
//    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
//    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */
//    
//    /* setup for non-canonical mode */
//    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
//    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
//    tty.c_oflag &= ~OPOST;
//    
//    /* fetch bytes as they become available */
//    tty.c_cc[VMIN] = 1;
//    tty.c_cc[VTIME] = 1;
//    
//    if (tcsetattr (fd, TCSANOW, &tty) != 0)
//    {
//        printf ("error %d from tcsetattr", errno);
//        return -1;
//    }
//    return 0;
//}
//
//void set_blocking (int fd, int should_block)
//{
//    struct termios tty;
//    memset (&tty, 0, sizeof tty);
//    if (tcgetattr (fd, &tty) != 0)
//    {
//        printf ("error %d from tggetattr", errno);
//        return;
//    }
//    
//    tty.c_cc[VMIN]  = should_block ? 1 : 0;
//    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout
//    
//    if (tcsetattr (fd, TCSANOW, &tty) != 0)
//        printf ("error %d setting term attributes", errno);
//}

int main(int argc, const char * argv[]) {
    char *portName = "/dev/tty.usbserial";
    struct termios tty;
    
    
    unsigned char message[5] = {0xFF, 0x00, 0x03, 0x1D, 0x0C};
    
    printf("fucking trying bitch\n");
    
    int fd = open ("/dev/cu.usbserial\0", 0x20006, 0x5);
    if (fd < 0) {
        printf("error %d openning %s : %s", errno, portName,strerror(errno) );
        exit(EXIT_FAILURE);
    }
    
    printf("openned\n");
    
    int ret = tcgetattr(fd, &tty);
    
    printf("tcgetattr : ok");
    
    if (ret == -1) {
        printf("error tcgetattr : %d %s", errno, strerror(errno));
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
    
    ret = tcsetattr(fd, TCSANOW, &tty);
    
    if (ret == -1 ) {
        printf("error tcsetattr %d %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    
    printf("starting writing\n");
    
    ret = (int) write(fd, message, 5);
    
    printf("write returned : %d\n", ret);
    
    printf("starting reading\n");
    
    unsigned char *buf = malloc(100 * sizeof(unsigned char));
    
    memset(buf, 0, 100);
    
    
    struct timeval tv;
    fd_set set;
    __DARWIN_FD_ZERO(&set);
    __DARWIN_FD_SET(fd, &set);
    tv.tv_sec = 1;
    tv.tv_usec = 1;
    
    ret = select(fd+1, &set, NULL, NULL, &tv);
    
    if (ret < 0) {
        printf("error select : %d %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    
    ret = (int)read(fd, buf, (ssize_t)100);
    
    if (ret < 0) {
        printf("error reading : %d %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    if (ret == 0) {
        printf("nothing read\n");
        exit(EXIT_FAILURE);
    }
    
    
    for (int i=0; i<100; i++) {
        printf("%x", buf[i]);

    }

    
    if (ret < 0) {
        printf("error reading : %d %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    if (ret == 0) {
        printf("nothing read\n");
        exit(EXIT_FAILURE);
    }
    
    
    for (int i=0; i<100; i++) {
        printf("%x", buf[i]);
        
    }
    
    
    return 0;
}


















