#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <kernel_types.h>

typedef struct node{
    PCB process;
    struct node * next;
} Node;

typedef Node * Queue;

int prepare_process_for_work(pid_t pid);

void scheduler_init(Pipe * stdin);
pid_t create_process(uint64_t rip, int argc, char * argv[]);
int terminate_process(int return_value, char autokill);
int block_process(pid_t process_pid);
int unblock_process(pid_t process_pid);
PCB * get_process(pid_t pid);
pid_t get_current_pid();
int change_priority(pid_t pid, int priority_value);
int yield_process();

PCBInfo * process_info();

#endif