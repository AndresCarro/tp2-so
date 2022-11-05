#ifndef PIPE_H
#define PIPE_H

#include <stdint.h>
#include <kernel_types.h>

Pipe * pipe_open();
void pipe_close(Pipe * pipe, char close_write_end);
int pipe_write(Pipe * p, char * str, int length);
int pipe_read(Pipe * p, char * str, int length);
void pipe_inherited(Pipe * p, char for_write);
PipeInfo * pipe_info();

#endif