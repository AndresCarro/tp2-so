#ifndef SYSLIB_H
#define SYSLIB_H

#include <defs.h>
#include <stdint.h>
#include <general_info.h>

#define NULL (void*)0

#define STDIN 0
#define STDOUT 1
#define STDERR 2


typedef void (*ptr)();
typedef ptr (*pm)();


#define EOF -1

typedef struct {
    uint8_t day, month, year;
    uint8_t hours, minutes, seconds;
} time_t;

typedef uint64_t sem_t;

int sys_write(int fd, const char *buffer, int size);
int sys_read(int fd,char * buffer,int size);
void sys_time(time_t * time);
int sys_getregs(uint64_t * registers);
void inv_opcode();
void div_zero();
void sys_execve(void (*f1)(), void (*f2)());
void sys_copymem(uint64_t address, uint8_t * buffer, uint64_t length);

pid_t exec(uint64_t program, unsigned int argc, char * argv[]);
pid_t waitpid(pid_t pid);
int nice(int new_priority);

sem_t sem_open(char * name, int initial_value);
void sem_close(sem_t sem);
int sem_wait(sem_t sem);
int sem_post(sem_t sem);

int pipe(int pipefds[]);
void dup2(int oldfd, int newfd);
void close(int fd);

unsigned int strlen(const char *s);
void excepDivZero();
void excepInvalidOpcode();
int puts(const char* s);
int putChar(char);
char getChar();
int gets(char * s);
unsigned int charBelongs(char *s,char c);
int containsString(const char *p1,const char *p2);
void savePrintMemParams(char *s);
int strcmp (const char *p1, const char *p2);
void getTime();
void printPrime();
void fibonacciNumbs();
void printmem();
void inforeg();
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
int atoi(char * str);
void itoa(int n, char s[]);

void fprintf(int fd, char * str, ...);

PipeInfo * pipe_info();
SemInfo * sem_info();
PCBInfo * process_info();
MemInfo * mem_info();

void * malloc(uint64_t size);
void free(void * ptr);

int kill(pid_t pid);
int block(pid_t pid);
int unblock(pid_t pid);

#endif