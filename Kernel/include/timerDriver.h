#ifndef timerDriver
#define timerDriver

#include <idtLoader.h>
#include <naiveConsole.h>

void timer_handler(uint64_t * registers);
int ticks_elapsed();
int seconds_elapsed();

#endif
