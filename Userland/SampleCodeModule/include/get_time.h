#ifndef GET_TIME_H
#define GET_TIME_H

#include <syslib.h>

typedef struct {
    uint8_t day, month, year;
    uint8_t hours, minutes, seconds;
} time_t;

void get_time();
void sys_time(time_t * time);

#endif
