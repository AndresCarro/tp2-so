#ifndef SCHEDULER_H2
#define SCHEDULER_H2

#include <blocked_queue.h>
#include <pipe2.h>
#include <stdint.h>
#include <general_info.h>

#define TOT_PRIORITIES 9
#define DEF_PRIORITY 5

#define READ 0
#define WRITE 1
#define CLOSED 2

#define MAX_FDS 16

typedef struct {
    unsigned int mode;
    Pipe * pipe;
} fd_t;

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

typedef struct node{
    PCB process;
    struct node * next;
} Node;

typedef Node * Queue;

void scheduler_init(Pipe * stdin);
pid_t create_process(uint64_t rip, int argc, char * argv[]);
int terminate_process(int return_value);
void block_process(pid_t process_pid);
void unblock_process(pid_t process_pid);
PCB * get_process(pid_t pid);
pid_t get_current_pid();
int change_priority(int priority_value);

PCBInfo * process_info();

#endif