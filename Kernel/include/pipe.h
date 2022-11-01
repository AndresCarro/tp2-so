#ifndef PIPE_H
#define PIPE_H

#include <blocked_queue.h>
#include <pipe2.h>
#include <scheduler2.h>
#include <stdint.h>
#include <general_info.h>

typedef struct pipenode {
    Pipe * p;
    struct pipenode * next;
} PipeNode;

typedef PipeNode * PipeList;

Pipe * pipe_open();
void pipe_close(Pipe * pipe, char close_write_end);
int pipe_write(Pipe * p, char * str, int length);
int pipe_read(Pipe * p, char * str, int length);

PipeInfo * pipe_info();

#endif