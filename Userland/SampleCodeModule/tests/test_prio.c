// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <test_util.h>
#include <syslib.h>
#include <syscalls.h>
#include <types.h>

#define WAIT 900000000 // Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 8 // Change as required
#define MEDIUM 4 // Change as required
#define HIGHEST 0 // Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_priority(int argc, char * argv[]) {
    int64_t pids[TOTAL_PROCESSES];
    uint64_t i;

    char * name = "Endless Loop Print";
    char * arg[] = {name};
    for (i = 0; i < TOTAL_PROCESSES; i++) {
        pids[i] = exec((uint64_t) endless_loop_print, 1, arg);
    }

    busy_wait(WAIT);
    fprintf(STDOUT, "CHANGING PRIORITIES...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++) {
        nice(pids[i], prio[i]);
    }

    busy_wait(WAIT);
    fprintf(STDOUT, "\nBLOCKING...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++) {
        block(pids[i]);
    }

    fprintf(STDOUT, "CHANGING PRIORITIES WHILE BLOCKED...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++) {
        nice(pids[i], MEDIUM);
    }

    fprintf(STDOUT, "UNBLOCKING...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++) {
        unblock(pids[i]);
    }

    busy_wait(WAIT);
    fprintf(STDOUT, "\nKILLING...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++) {
        kill(pids[i]);
    }
}