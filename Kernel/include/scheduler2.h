#ifndef SCHEDULER_H2
#define SCHEDULER_H2

#include <blocked_queue.h>
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
    BlockedQueueADT blocked_queue;
} PCB;

typedef struct node{
    PCB process;
    struct node * next;
} Node;

typedef Node * Queue;

void scheduler_init();
pid_t create_process(uint64_t rip, int argc, char * argv[]);
int terminate_process(int return_value);
void block_process(pid_t process_pid);
void unblock_process(pid_t process_pid);
PCB * get_process(pid_t pid);
pid_t get_current_pid();

#endif