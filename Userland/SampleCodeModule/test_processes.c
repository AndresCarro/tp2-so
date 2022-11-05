#include <test_util.h>
#include <syslib.h>
#include <syscalls.h>
#include <types.h>

enum State {RUNNING, BLOCKEDD, KILLED};

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

void test_processes(int argc, char * argv[]) {
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;
    uint64_t max_processes;
    char * name = "Endless Loop";
    char * arg[] = {name};

    if (argc != 2) {
        fprintf(STDOUT, "ERROR: Invalid amount of arguments provided.");
        return;
    }

    if ((max_processes = atoi(argv[1])) <= 0) {
        fprintf(STDOUT, "ERROR: Invalid argument.");
        return;
    }

    p_rq p_rqs[max_processes];

    int j = 0;
    while (j < 10) {
        // Create max_processes processes
        for (rq = 0; rq < max_processes; rq++) {
            p_rqs[rq].pid = exec((uint64_t) endless_loop, 1, arg);

            if (p_rqs[rq].pid == -1) {
                fprintf(STDOUT, "test_processes: ERROR creating process\n");
                return;
            } else {
                fprintf(STDOUT, "test_processes: process %d CREATED\n", p_rqs[rq].pid);
                p_rqs[rq].state = RUNNING;
                alive++;
            }
        }

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0) {
            for (rq = 0; rq < max_processes; rq++) {
                action = get_uniform(100) % 2; 

                switch(action) {
                    case 0:
                        if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKEDD) {
                            if (kill(p_rqs[rq].pid) == -1) {  
                                fprintf(STDOUT, "test_processes: ERROR killing process\n");
                                return;
                            } else {
                                fprintf(STDOUT, "test_processes: process %d KILLED\n", p_rqs[rq].pid);
                            }
                            p_rqs[rq].state = KILLED; 
                            alive--;
                        }
                        break;
                    case 1:
                        if (p_rqs[rq].state == RUNNING) {
                            if (block(p_rqs[rq].pid) == -1) {
                                fprintf(STDOUT, "test_processes: ERROR blocking process\n");
                                return;
                            } else {
                                fprintf(STDOUT, "test_processes: process %d BLOCKED\n", p_rqs[rq].pid);
                            }
                            p_rqs[rq].state = BLOCKEDD; 
                        }
                        break;
                }
            }

            // Randomly unblocks processes
            for(rq = 0; rq < max_processes; rq++) {
                if (p_rqs[rq].state == BLOCKEDD && get_uniform(100) % 2) {
                    if(unblock(p_rqs[rq].pid) == -1) {
                        fprintf(STDOUT, "test_processes: ERROR unblocking process\n");
                        return;
                    } else {
                        fprintf(STDOUT, "test_processes: process %d UNBLOCKED\n", p_rqs[rq].pid);
                    }
                    p_rqs[rq].state = RUNNING; 
                }
            }
        } 
        j++;
    }
    fprintf(STDOUT, "All good with test_processes. ;)\n");
}