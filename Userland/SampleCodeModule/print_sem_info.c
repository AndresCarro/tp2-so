#include <print_sem_info.h>

void print_sem_info() {
    SemInfo * info = sem_info();
    if(info == NULL){
        puts("There are no active semaphores.\n");
        return;
    }
    while (info != NULL) {
        fprintf(STDOUT, "Name: %s, Value: %d, Blocked Processes: %d, Linked Processes: %d\n", info->name, info->value, info->blocked_processes, info->linked_processes);
        info = info->next;
    }
}