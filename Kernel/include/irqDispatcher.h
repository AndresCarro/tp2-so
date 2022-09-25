#ifndef IRQDISPATCHER_H
#define IRQDISPATCHER_H
#include <stdint.h>
#include <keyboardDriver.h>
#include <timerDriver.h>
#include <stdint.h>

void irqDispatcher(uint64_t irq, uint64_t * registers);

#endif