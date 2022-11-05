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
    while (info != NULL) {
        fprintf(STDOUT, "Name: %s; Value: %d; Blocked Processes: %d; Linked Processes: %d\n", info->name, info->value, info->blocked_processes, info->linked_processes);
        info = info->next;
    }
    while (info != NULL) {
        SemInfo * aux = info;
        info = info->next;
        free((uint64_t) aux->name);
        free((uint64_t) aux);
    }
}