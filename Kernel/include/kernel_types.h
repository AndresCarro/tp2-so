#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

#include <stdint.h>
#include <stddef.h>

/* Process Status */
#define READY 0
#define BLOCKED 1

#define PIPESIZE 512

#define READ 0
#define WRITE 1
#define CLOSED 2

#define STDIN 0
#define STDOUT 1

#define MAX_FDS 16

#define TOT_PRIORITIES 9
#define DEF_PRIORITY 4

/* Scheduler */
typedef int pid_t;
typedef unsigned int priority_t;
typedef unsigned int status_t;

/* Blocked Queue */
typedef struct blockednode {
    pid_t pid;
    struct blockednode * next;
} BlockedNode;

typedef struct {
    BlockedNode * first;
    BlockedNode * last;
    unsigned int qty;
} BlockedQueueCDT;

typedef BlockedQueueCDT * BlockedQueueADT;

/* Pipe */
typedef struct {
    char data[PIPESIZE];
    unsigned int read_offset;
    unsigned int write_offset;
    unsigned int open_for_read;
    unsigned int open_for_write;
    BlockedQueueADT blocked_in_read;
    BlockedQueueADT blocked_in_write;
} Pipe;

typedef struct pipenode {
    Pipe * p;
    struct pipenode * next;
} PipeNode;

typedef PipeNode * PipeList;

/* File Descriptor */
typedef struct {
    unsigned int mode;
    Pipe * pipe;
} fd_t;

/* Scheduler */
typedef struct {
    pid_t pid;
    priority_t priority;
    int new_priority;
    status_t status;
    unsigned int quantums_left;
    uint64_t rsp;
    uint64_t stack_base;
    BlockedQueueADT blocked_queue;
    fd_t file_desciptors[MAX_FDS];
    unsigned int last_fd;
    unsigned int argc;
    char ** argv;
} PCB;

/* Semaphores */
typedef struct {
    char * name;
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

/* Time */
typedef struct {
    uint8_t day, month, year;
    uint8_t hours, minutes, seconds;
} time_t;

/* Information */
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
    uint64_t memory_free;
    uint64_t memory_occupied;
    uint64_t memory_total;
    unsigned int memory_frags;
} MemInfo;

#endif