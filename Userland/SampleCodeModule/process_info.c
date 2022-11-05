#include <processes.h>
#include <types.h>
#include <syscalls.h>
#include <syslib.h>

static char * status[] = {"Ready", "Blocked"};

void ps(int argc, char * argv[]) {
    PCBInfo * info = process_info();
    fprintf(STDOUT, "PROCESSES INFORMATION\n");
    while (info != NULL) {
        fprintf(STDOUT, "Name: %s; PID: %d; RSP: 0x%x, RBP: 0x%x; Priority: %d; Status: %s\n", info->name, info->pid, info->rsp, info->rbp, info->priority, status[info->status]);
        info = info->next;
    }
    while (info != NULL) {
        PCBInfo * aux = info;
        info = info->next;
        free((uint64_t) aux->name);
        free((uint64_t) aux);
    }
}