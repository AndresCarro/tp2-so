#ifndef SYSLIB_H
#define SYSLIB_H

#include <types.h>
#include <stdint.h>

uint64_t read(unsigned int fd, char* output, uint64_t count);
void write(unsigned fd, const char* buffer, uint64_t count);
pid_t exec(uint64_t program, unsigned int argc, char * argv[]);
void exit(int retValue);
pid_t getpid();
pid_t waitpid(pid_t pid);
int yield();
int block(pid_t pid);
int unblock(pid_t pid);
int kill(pid_t pid);
int nice(pid_t pid, int new_priority);
void * malloc(uint64_t size);
void free(uint64_t ptr);
sem_t sem_open(char * name, int initial_value);
void sem_close(sem_t sem);
int sem_wait(sem_t sem);
int sem_post(sem_t sem);
int pipe(int fds[]);
void dup2(int old, int new);
void close(int fd);
MemInfo * mem_info();
PipeInfo * pipe_info();
SemInfo * sem_info();
PCBInfo * process_info();

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


#endif