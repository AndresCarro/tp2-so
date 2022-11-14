// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>
#include <syscalls.h>
#include <types.h>
#include <syslib.h>

#define MAX_PHILOS 10
#define INITIAL_PHILOS 3

#define THINKING 0
#define EATING 1
#define HUNGRY 2
 
int state[MAX_PHILOS];
int philos[MAX_PHILOS] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
pid_t pids[MAX_PHILOS];

char terminating = 0; 
int philo_count = 0;

sem_t sems[MAX_PHILOS];

void wait(int times) {
    for (int i = 0; i < times; i++) {
        for (int j = 0; j < WAIT_TIME; j++);
    }
}

int get_left(int philo) {
    return (philo + philo_count - 1) % philo_count;
}

int get_right(int philo) {
    return (philo + 1) % philo_count;
}
 
void try_eat(int philo) {
    if (state[philo] == HUNGRY && state[get_left(philo)] != EATING && state[get_right(philo)] != EATING) {
        state[philo] = EATING;

        wait(2);

        for (int i = 0; i < philo; i++) {
            fprintf(STDOUT, "* ");
        }
        fprintf(STDOUT, "%c ", philos[philo]);
        for (int i = 0; i < philo_count - (philo + 1); i++) {
            fprintf(STDOUT, "* ");
        }
        put_char('\n');
 
        sem_post(sems[philo]);
        state[philo] = THINKING;
    }
}
 
void take_fork(int philo) {
    state[philo] = HUNGRY;
    try_eat(philo); 
    sem_wait(sems[philo]);
 
    wait(1);
}
 
void put_fork(int philo) {  
    try_eat(get_left(philo));
    try_eat(get_right(philo));
}

void * philosopher(int argc, char * argv[]) {
    int i = atoi(argv[1]);

    while (1) {
        take_fork(i);
        wait(1);
        put_fork(i);
    }
}

void add_philo() {
    char sem_name[2] = {0, 0};
    sem_name[0] = philos[philo_count];
    sems[philo_count] = sem_open(sem_name, 0);

    char name[14] = "Phylosopher X";
    name[12] = philos[philo_count];
    char num[2];
    itoa(philo_count, num);
    char * argv_p[] = {name, num};
    pids[philo_count] = exec((uint64_t) philosopher, 2, argv_p);

    philo_count++;
}

void delete_philo() {
    philo_count--;
    sem_close(sems[philo_count]);
    if (!terminating && (state[philo_count] == EATING || state[philo_count] == THINKING)) {
        try_eat(get_left(philo_count));
        try_eat(get_right(philo_count));
    }
    kill(pids[philo_count]);
}

void terminate() {
    terminating = 1;
    while (philo_count > 0) {
        delete_philo();
    }
}

void phylo(int argc, char * argv[]) {
    fprintf(STDOUT, "Press 'a' to add a phylosopher.\nPress 'r' to remove a phylosopher.\nPress 'q' to exit.\n");

    for (int i = 0; i < INITIAL_PHILOS; i++) {
        add_philo();
    }

    while (1) {
        char c = get_char();
        if (c == 'a' || c == 'A') {
            if (philo_count < MAX_PHILOS) {
                add_philo();
            } else {
                fprintf(STDOUT, "ERROR: Can't add more phylosophers.\n");
            }
        } else if (c == 'r' || c == 'R') {
            if (philo_count > 1) {
                delete_philo();
            } else {
                fprintf(STDOUT, "ERROR: Can't remove more phylosophers.\n");
            }
        } else if (c == 'q' || c == 'Q' || c == -1) {
            terminate();
            return;
        }
    }
}