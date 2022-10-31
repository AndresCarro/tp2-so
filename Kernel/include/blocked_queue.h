#ifndef BLOCKED_QUEUE_H
#define BLOCKED_QUEUE_H

#include <defs.h>
#include <memory_manager.h>

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

BlockedQueueADT new_blocked_queue();
pid_t dequeue_pid(BlockedQueueADT queue);
void enqueue_pid(BlockedQueueADT queue, pid_t pid);
void free_queue(BlockedQueueADT queue);
int is_empty(BlockedQueueADT queue);
unsigned int get_blocked_qty(BlockedQueueADT queue);

#endif