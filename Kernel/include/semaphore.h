#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <scheduler2.h>
#include <blocked_queue.h>
#include <stdint.h>
#include <lib.h>
#include <general_info.h>

typedef struct {
    const char * name;
    uint8_t value;
    uint8_t lock;
    unsigned int linked_processes;
    BlockedQueueADT blocked_processes; 
} Semaphore;

typedef struct sem_node {
    Semaphore sem;
    struct sem_node * next;
} SemNode;

typedef Semaphore * sem_t;
typedef SemNode * SemList;

int sem_wait(sem_t sem);
int sem_post(sem_t sem);
void sem_close(sem_t sem);
sem_t sem_open(const char * name, uint8_t initial_value);
SemInfo * sem_info();

#endif