// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>
#include <types.h>
#include <syscalls.h>
#include <syslib.h>

static char * status[] = {"Ready", "Blocked"};

void ps(int argc, char * argv[]) {
    PCBInfo * info = process_info();
    fprintf(STDOUT, "%-20s%-5s%-12s%-12s%-10s%-8s\n", "NAME", "PID", "RSP", "RBP", "PRIORITY", "STATUS");
    while (info != NULL) {
        fprintf(STDOUT, "%-20s%-5d0x%-10x0x%-10x%-10d%-8s\n", info->name, info->pid, info->rsp, info->rbp, info->priority, status[info->status]);
        info = info->next;
    }
    while (info != NULL) {
        PCBInfo * aux = info;
        info = info->next;
        free((uint64_t) aux->name);
        free((uint64_t) aux);
    }
}