#ifndef BLOCKED_QUEUE_H
#define BLOCKED_QUEUE_H

#include <kernel_types.h>

BlockedQueueADT new_blocked_queue();
pid_t dequeue_pid(BlockedQueueADT queue);
void enqueue_pid(BlockedQueueADT queue, pid_t pid);
void free_queue(BlockedQueueADT queue);
int is_empty(BlockedQueueADT queue);
unsigned int get_blocked_qty(BlockedQueueADT queue);

#endif