#include <get_time.h>

void get_time(){
    time_t time;
    char buffer[64] = {'0'};
    sys_time(&time);

    put_char('\n');
    uintToBase(time.hours, buffer, 10);
    puts(buffer);
    put_char(':');
    uintToBase(time.minutes, buffer, 10);
    puts(buffer);
    put_char(':');
    uintToBase(time.seconds, buffer, 10);
    puts(buffer);
    put_char('\n');

    uintToBase(time.day, buffer, 10);
    puts(buffer);
    put_char('/');
    uintToBase(time.month, buffer, 10);
    puts(buffer);
    put_char('/');
    uintToBase(time.year+2000, buffer, 10);
    puts(buffer);
    put_char('\n');
    
}