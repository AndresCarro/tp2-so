// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>
#include <types.h>
#include <syslib.h>
#include <syscalls.h>

void loop(int argc, char * argv[]) {
    pid_t pid = getpid();
    int i = 0;
    while (1) {
        if (i == WAIT_TIME) {
            fprintf(STDOUT, "Hello there! I'm %d :)\n",pid);
            i = 0;
        }
        i++;
    }
}