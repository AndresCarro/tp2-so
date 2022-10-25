#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <memory_manager.h>
#include <stdint.h>
#include <defs.h>

#define TOT_PRIORITIES 9
#define DEF_PRIORITY 5


typedef struct {
    pid_t pid;
    priority_t priority;
    status_t status;
    unsigned int quantums_left;
    uint64_t rsp;
    uint64_t stack_base;
} PCB;

typedef struct node{
    PCB process;
    struct node * next;
} Node;

typedef Node * Queue;

pid_t create_process(uint64_t rip, int argc, char * argv[]);

#endif