#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <stdint.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();

#endif
