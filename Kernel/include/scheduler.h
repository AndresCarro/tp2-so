#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <naiveConsole.h>
#include <interrupts.h>
#include <timerDriver.h>
#include <defs.h>

void _defaultExit();

void loadTasks(int (*program1)(), int (*program2)(), uint64_t * registers);

void nextTask(uint64_t * registers);

void changeStatus(unsigned int taskNum);

void exitTask(int retValue, uint64_t * registers);

void terminateTasks();

int getCurrentTask();

#endif //SCHEDULER_H
