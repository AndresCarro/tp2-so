#include <print_process_info.h>

void print_process_info() {
    PCBInfo * info = process_info();
    char * status[] = {"Ready", "Blocked", "Terminated"};
    while (info != NULL) {
        fprintf(STDOUT, "Name: %s, PID: %d, RSP: 0x%x, RBP: 0x%x, Priority: %d, Status: %s\n", info->name, info->pid, info->rsp, info->rbp, info->priority, status[info->status]);
        info = info->next;
    }
}