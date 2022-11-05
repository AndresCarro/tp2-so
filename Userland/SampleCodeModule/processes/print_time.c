#include <processes.h>
#include <syslib.h>
#include <syscalls.h>
#include <types.h>

void print_time(int argc, char * argv[]) {
    time_t t;
    time(&t);

    fprintf(STDOUT, "%d:%d:%d\n%d/%d/%d\n", t.hours, t.minutes, t.seconds, t.day, t.month, t.year + 2000);
}