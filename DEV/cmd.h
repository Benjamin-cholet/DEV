//
//  cmd.h
//  DEV
//
//  Created by Benjamin on 17/02/2017.
//  Copyright © 2017 Benjamin. All rights reserved.
//

#ifndef cmd_h
#define cmd_h

#include <stdio.h>

void CRC16_CCITT(uint16_t *crcReg, unsigned char data);
int checkCRC(unsigned char *msg, int lenth);

#endif /* cmd_h */
