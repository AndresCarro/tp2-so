#ifndef PIPE_H2
#define PIPE_H2

#include <blocked_queue.h>

#define PIPESIZE 512

typedef struct {
    char data[PIPESIZE];
    unsigned int read_offset;
    unsigned int write_offset;
    unsigned int open_for_read;
    unsigned int open_for_write;
    BlockedQueueADT blocked_in_read;
    BlockedQueueADT blocked_in_write;
} Pipe;

void pipe_inherited(Pipe * p, char for_write);

#endif