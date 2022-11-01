#include <stdint.h>
#include <test_util.h>
#include <syslib.h>

#define WAIT    900000000 // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 8 //TODO: Change as required
#define MEDIUM 4 //TODO: Change as required
#define HIGHEST 0 //TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio(int argc, char * argv[]) {
    int64_t pids[TOTAL_PROCESSES];
    char * name = "Endless Loop Print";
    char * arg[] = {name};
    uint64_t i;

    for(i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = exec(endless_loop_print, 1, arg);

    bussy_wait(WAIT);
    fprintf(STDOUT, "\nCHANGING PRIORITIES...\n");

    // Al primero la mas baja osea 8 -> 2
    // Al segundo la del medio osea 4 -> 3
    // Al tercero la mas alta osea 0 -> 4
    for(i = 0; i < TOTAL_PROCESSES; i++)
        nice(pids[i], prio[i]);

    bussy_wait(WAIT);
    fprintf(STDOUT, "\nBLOCKING...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++)
        block(pids[i]);

    fprintf(STDOUT, "CHANGING PRIORITIES WHILE BLOCKED...\n");

    // A todo le pone prioridad media, es decir, todos lo mismo.
    for(i = 0; i < TOTAL_PROCESSES; i++)
        nice(pids[i], MEDIUM);

    fprintf(STDOUT, "UNBLOCKING...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++)
        unblock(pids[i]);

    bussy_wait(WAIT);
    fprintf(STDOUT, "\nKILLING...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++)
        kill(pids[i]);
}