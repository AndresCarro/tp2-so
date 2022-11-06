#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <types.h>

extern uint64_t read(unsigned int fd, char* output, uint64_t count);
extern void write(unsigned fd, const char* buffer, uint64_t count);
extern pid_t exec(uint64_t program, unsigned int argc, char * argv[]);
extern void exit(int retValue);
extern pid_t getpid();
extern pid_t waitpid(pid_t pid);
extern int yield();
extern int block(pid_t pid);
extern int unblock(pid_t pid);
extern int kill(pid_t pid);
extern int nice(pid_t pid, int new_priority);
extern void * malloc(uint64_t size);
extern void free(uint64_t ptr);
extern sem_t sem_open(char * name, int initial_value);
extern void sem_close(sem_t sem);
extern int sem_wait(sem_t sem);
extern int sem_post(sem_t sem);
extern int pipe(int fds[]);
extern void dup2(int old, int new);
extern void close(int fd);
extern MemInfo * mem_info();
extern PipeInfo * pipe_info();
extern SemInfo * sem_info();
extern PCBInfo * process_info();
extern void time(date_t * s);
extern void clear();

#endif