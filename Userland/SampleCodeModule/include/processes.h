#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdint.h>

uint64_t test_mm(int argc, char * argv[]);
int64_t test_processes(int argc, char * argv[]);
void test_prio(int argc, char * argv[]);
uint64_t test_sync(uint64_t argc, char *argv[]);

#endif