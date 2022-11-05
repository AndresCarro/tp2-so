#include <processes.h>
#include <types.h>
#include <syslib.h>
#include <syscalls.h>

void pipes(int argc, char * argv[]) {
    PipeInfo * info = pipe_info();
    fprintf(STDOUT, "PIPE INFORMATION\n");
    if (info == NULL) {
        fprintf(STDOUT, "There are no active pipes.\n");
        return;
    }
    while (info != NULL) {
        fprintf(STDOUT, "Bytes Available: %d; Bytes Left to Read: %d; # Open for Read: %d; # Open for Write: %d\n", info->available_space, info->left_to_read, info->open_for_read, info->open_for_write);
        info = info->next;
    }
    while (info != NULL) {
        PipeInfo * aux = info;
        info = info->next;
        free((uint64_t) aux);
    }
}