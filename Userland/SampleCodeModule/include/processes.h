#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdint.h>

uint64_t test_mm(int argc, char * argv[]);
int64_t test_processes(int argc, char * argv[]);
void test_prio(int argc, char * argv[]);
uint64_t test_sync(uint64_t argc, char *argv[]);
void print_mem_info();
void print_process_info();
void print_sem_info();
void print_pipe_info();
void loop();

void write_handler(int argc,char* argv[]);
void read_handler(int argc,char* argv[]);
void background_handler(int argc,char* argv[]);

void print_prime();
void help();
void get_time();
void fibonacci_numbs();
void wc();

void excepDivZero();
void excepInvalidOpcode();

#endif