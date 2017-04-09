//
//  main.h
//  DEV
//
//  Created by Benjamin on 16/02/2017.
//  Copyright © 2017 Benjamin. All rights reserved.
//

#ifndef main_h
#define main_h

#ifdef DEBUG
#define PRINTD printf
#else
#define PRINTD(...) ((void)0)
#endif

void init(int fd);

#endif /* main_h */
