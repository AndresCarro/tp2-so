#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <lib.h>
#include <keyboardDriver.h>
#include <scheduler.h>
#include <inforeg.h>
#include <defs.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef struct {
    uint8_t day, month, year;
    uint8_t hours, minutes, seconds;
} time_t;

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax, uint64_t * registers);

#endif