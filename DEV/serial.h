//
//  serial.h
//  DEV
//
//  Created by Benjamin on 16/02/2017.
//

#ifndef serial_h
#define serial_h

void serial_init(int *fd);
void serial_write(int fd, unsigned char *msg);
void serial_read(int fd, unsigned char **msg, int *length);
void serial_read_to_stdout(int fd);

#endif /* serial_h */
