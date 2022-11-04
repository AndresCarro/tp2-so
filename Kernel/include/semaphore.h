#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include <kernel_types.h>

int sem_wait(sem_t sem);
int sem_post(sem_t sem);
void sem_close(sem_t sem);
sem_t sem_open(char * name, uint8_t initial_value);
SemInfo * sem_info();

#endif