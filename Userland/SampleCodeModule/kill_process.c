#include <kill_process.h>

void kill_process(pid_t pid){
    if( kill(pid) == -1){
        fprintf(STDOUT,"Couldn't kill process %d\n",pid);
    }
}