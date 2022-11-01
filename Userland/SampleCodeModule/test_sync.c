#include <stdint.h>
#include <test_util.h>
#include <syslib.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 10

int64_t global;  //shared memory

// Le incrementa un numero que le pasan de manera lenta.
void slowInc(int64_t *p, int64_t inc){
  uint64_t aux = *p;
  yield(); //This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]){
    uint64_t n;
    int8_t inc;
    int8_t use_sem;

    if (argc != 4) return -1;

    if ((n = atoi(argv[1])) <= 0) return -1;
    if ((inc = atoi(argv[2])) == 0) return -1;
    if ((use_sem = atoi(argv[3])) < 0) return -1;

    sem_t sem;
    if (use_sem) {
        if ((sem = sem_open(SEM_ID, 1)) == NULL){
            fprintf(STDOUT, "test_sync: ERROR opening semaphore\n");
            return -1;
        }
    }

    uint64_t i;
    for (i = 0; i < n; i++){
        if (use_sem) sem_wait(sem);
        slowInc(&global, inc);
        if (use_sem) sem_post(sem);
    }

    if (use_sem) sem_close(sem);
    
    return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]){ //{n, use_sem}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES]; // 4 procesos


  if (argc != 3) return -1;

  char * argvDec[] = {"Decrement", argv[1], "-1", argv[2]};
  char * argvInc[] = {"Increment", argv[1], "1", argv[2]};

  global = 0;

  uint64_t i;
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    pids[i] = exec(my_process_inc, 4, argvDec);
    pids[i + TOTAL_PAIR_PROCESSES] = exec(my_process_inc, 4, argvInc);
  }

  fprintf(STDOUT, "SE CREARON TODOS\n");
  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    waitpid(pids[i]);
    fprintf(STDOUT, "EL PID %d TERMINO\n", pids[i]);
    waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
    fprintf(STDOUT, "EL PID %d TERMINO\n", pids[i + TOTAL_PAIR_PROCESSES]);
  }

  fprintf(STDOUT, "Final value: %d\n", global);

  return 0;
}