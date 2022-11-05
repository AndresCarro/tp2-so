#include <stdint.h>
#include <test_util.h>
#include <syslib.h>
#include <syscalls.h>
#include <types.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 10

int64_t global;  //shared memory

void slow_inc(int64_t * p, int64_t inc) {
    uint64_t aux = *p;
    yield(); //This makes the race condition highly probable
    aux += inc;
    *p = aux;
}

void internal_test_synchro(int argc, char *argv[]) {
    uint64_t n;
    int8_t inc;
    int8_t use_sem;

    if (argc != 4) {
        return;
    }

    if ((n = atoi(argv[1])) <= 0) {
        return;
    }
    if ((inc = atoi(argv[2])) == 0) {
        return;
    }
    if ((use_sem = atoi(argv[3])) < 0) {
        return;
    }

    sem_t sem;
    if (use_sem) {
        if ((sem = sem_open(SEM_ID, 1)) == 0) {
            fprintf(STDOUT, "test_sync: ERROR opening semaphore\n");
            return;
        }
    }

    uint64_t i;
    for (i = 0; i < n; i++) {
        if (use_sem) {
            sem_wait(sem);
        }
        slow_inc(&global, inc);
        if (use_sem) {
            sem_post(sem);
        }
    }

    if (use_sem) {
        sem_close(sem);
    }
}

void test_synchro(int argc, char *argv[]) {
    uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

    if (argc != 3) {
        fprintf(STDOUT, "ERROR: Invalid amount of arguments provided.");
        return;
    } 

    char * argv_dec[] = {"Decrement Test Syncrho", argv[1], "-1", argv[2]};
    char * argv_inc[] = {"Increment Test Synchro", argv[1], "1", argv[2]};

    global = 0;

    uint64_t i;
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
        pids[i] = exec((uint64_t) internal_test_synchro, 4, argv_dec);
        pids[i + TOTAL_PAIR_PROCESSES] = exec((uint64_t) internal_test_synchro, 4, argv_inc);
    }

    fprintf(STDOUT, "test_sync: All processes created\n");
    for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
        waitpid(pids[i]);
        fprintf(STDOUT, "test_sync: PID %d finished\n", pids[i]);
        waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
        fprintf(STDOUT, "test_sync: PID %d finished\n", pids[i + TOTAL_PAIR_PROCESSES]);
    }

    fprintf(STDOUT, "test_sync: Final value: %d\n", global);
}