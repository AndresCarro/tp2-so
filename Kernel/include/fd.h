#ifndef FD_H
#define FD_H

#include <pipe.h>

#define READ 0
#define WRITE 1

#define MAX_FDS 16

typedef struct {
    unsigned int mode;
    Pipe * pipe;
} fd_t;

#endif