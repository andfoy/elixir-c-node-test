
#include <stdio.h>
#include <stdlib.h>

#include "ei.h"

#ifndef __UTILS_H
#define __UTILS_H

void process_tuple(char* buff, int *index, int size, int fd);
int my_listen(int port);
int foo(int x);
int bar(int x);

#endif  // _UTILS_H