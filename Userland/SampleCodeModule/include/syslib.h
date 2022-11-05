#ifndef SYSLIB_H
#define SYSLIB_H

#include <types.h>
#include <stdint.h>

#include <general_info.h>
#include <primes.h>
#include <help.h>
#include <invalid_opcode.h>
#include <exception_div_zero.h>
#include <get_time.h>
#include <print_mem_info.h>
#include <print_process_info.h>
#include <print_sem_info.h>
#include <print_pipe_info.h>
#include <loop.h>

#define NULL (void*)0

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define MAX_BUFFER 256

typedef void (* command)(int, char **);

#define EOF -1

typedef uint64_t sem_t;

char ** strtok(char * str, char delim, int * qty);

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
int is_num(char c);
int puts(const char* s);
int put_char(char);
char get_char();
int gets(char * s);
unsigned int char_belongs(char *s,char c);
int contains_string(const char *p1,const char *p2);
int strcmp (const char *p1, const char *p2);
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
int atoi(char * str);
void itoa(int n, char s[]);
void fprintf(int fd, char * str, ...);


#endif