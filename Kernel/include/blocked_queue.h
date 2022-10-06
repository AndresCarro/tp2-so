#ifndef BLOCKED_QUEUE_H
#define BLOCKED_QUEUE_H

#include <defs.h>

typedef struct blockednode {
    pid_t pid;
    struct blockednode * next;
} BlockedNode;

typedef struct {
    BlockedNode * first;
    BlockedNode * last;
} BlockedQueueCDT;

typedef BlockedQueueCDT * BlockedQueueADT;


#endif