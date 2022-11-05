#include <processes.h>
#include <types.h>
#include <syslib.h>
#include <syscalls.h>

void sem(int argc, char * argv[]) {
    SemInfo * info = sem_info();
    fprintf(STDOUT, "SEMAPHORES INFORMATION\n");
    if (info == NULL) {
        fprintf(STDOUT, "There are no active semaphores.\n");
        return;
    }
    fprintf(STDOUT, "%-20s%-10s%-20s%-20s\n", "NAME", "VALUE", "BLOCKED PROCESSES", "LINKED PROCESSES");
    while (info != NULL) {
        fprintf(STDOUT, "%-20s%-10d%-20d%-20d\n", info->name, info->value, info->blocked_processes, info->linked_processes);
        info = info->next;
    }
    while (info != NULL) {
        SemInfo * aux = info;
        info = info->next;
        free((uint64_t) aux->name);
        free((uint64_t) aux);
    }
}