//
//  serial.h
//  DEV
//
//  Created by Benjamin on 16/02/2017.
//  Copyright © 2017 Benjamin. All rights reserved.
//

#ifndef serial_h
#define serial_h

static const char *portName = "/dev/cu.usbserial";

void serial_init(int *fd);
void serial_write(int fd, unsigned char *msg);
void serial_read(int fd, unsigned char **msg, int *length);

#endif /* serial_h */
