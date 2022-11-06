#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

#define MAX_BUFFER 256

#define STDIN 0
#define STDOUT 1

#define EOF -1

typedef void (* command)(int, char **);

typedef int pid_t;
typedef unsigned int status_t;
typedef uint64_t sem_t;

typedef struct {
    uint8_t day, month, year;
    uint8_t hours, minutes, seconds;
} date_t;

typedef struct pcbinfo {
    char * name;
    pid_t pid;
    uint64_t rsp;
    uint64_t rbp;
    unsigned int priority;
    status_t status;
    struct pcbinfo * next;
} PCBInfo;

typedef struct pipeinfo {
    unsigned int available_space;
    unsigned int left_to_read;
    unsigned int open_for_read;
    unsigned int open_for_write;
    struct pipeinfo * next;
} PipeInfo;

typedef struct seminfo {
    char * name;
    uint8_t value;
    unsigned int linked_processes;
    unsigned int blocked_processes;
    struct seminfo * next;
} SemInfo;

typedef struct meminfo {
    char * mem_algorithm;
    uint64_t memory_free;
    uint64_t memory_occupied;
    uint64_t memory_total;
    unsigned int memory_frags;
} MemInfo;

#endif