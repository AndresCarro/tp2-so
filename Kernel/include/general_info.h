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
    
} SemInfo;

typedef struct meminfo {

} MemInfo;

#endif