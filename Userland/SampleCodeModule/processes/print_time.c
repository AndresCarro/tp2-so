#include <processes.h>
#include <syslib.h>
#include <syscalls.h>
#include <types.h>

void print_time(int argc, char * argv[]) {
    time_t t;
    char buffer[64] = {'0'};
    time(&t);

    fprintf(STDOUT, "TIME: %d\n", t.hours);

    put_char('\n');
    uint_to_base(t.hours, buffer, 10);
    fprintf(STDOUT,buffer);
    put_char(':');
    uint_to_base(t.minutes, buffer, 10);
    fprintf(STDOUT,buffer);
    put_char(':');
    uint_to_base(t.seconds, buffer, 10);
    fprintf(STDOUT,buffer);
    put_char('\n');

    uint_to_base(t.day, buffer, 10);
    fprintf(STDOUT,buffer);
    put_char('/');
    uint_to_base(t.month, buffer, 10);
    fprintf(STDOUT,buffer);
    put_char('/');
    uint_to_base(t.year+2000, buffer, 10);
    fprintf(STDOUT,buffer);
    put_char('\n');
    
}