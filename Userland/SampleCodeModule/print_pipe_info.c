#include <print_pipe_info.h>

void print_pipe_info(){
    PipeInfo * info = pipe_info();
    if( info == NULL){
        puts("There are no active pipes.\n");
        return;
    }
    while (info != NULL) {
        fprintf(STDOUT, "Avail: %d; Left: %d, Open Read: %d, Open Write: %d\n", info->available_space, info->left_to_read, info->open_for_read, info->open_for_write);
        info = info->next;
    }
}