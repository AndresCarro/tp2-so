#ifndef GENERAL_INFO_H
#define GENERAL_INFO_H

typedef struct pcbinfo {

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

} MemInfo;

#endif