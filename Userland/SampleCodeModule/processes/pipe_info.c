#include <processes.h>
#include <types.h>
#include <syslib.h>
#include <syscalls.h>

void pipes(int argc, char * argv[]) {
    PipeInfo * info = pipe_info();
    if (info == NULL) {
        fprintf(STDOUT, "There are no active pipes.\n");
        return;
    }
    fprintf(STDOUT, "%-20s%-25s%-18s%-16s\n", "BYTES AVAILABLE", "BYTES LEFT TO READ", "# OPEN FOR READ", "# OPEN FOR WRITE");
    while (info != NULL) {
        fprintf(STDOUT, "%-20d%-25d%-18d%-16d\n", info->available_space, info->left_to_read, info->open_for_read, info->open_for_write);
        info = info->next;
    }
    while (info != NULL) {
        PipeInfo * aux = info;
        info = info->next;
        free((uint64_t) aux);
    }
}