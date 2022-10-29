#ifndef PIPE_H
#define PIPE_H

#include <memory_manager.h>
#include <blocked_queue.h>
#include <stdint.h>
#include <defs.h>
#include <scheduler2.h>

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

Pipe * pipe_open();
void pipe_close(Pipe * pipe, char close_write_end);
int pipe_write(Pipe * p, char * str, int length);
int pipe_read(Pipe * p, char * str, int length);

#endif