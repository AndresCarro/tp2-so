#include <processes.h>
#include <types.h>
#include <syslib.h>

void fibonacci(int argc, char * argv[]) {
    char num[30];
    int t_1 = 0, t_2 = 1;
    long next_term = t_1 + t_2;
    fprintf(STDOUT, "Fibonacci Series:\n0,\n1,\n");
    while (1) {
        itoa(next_term, num);
        fprintf(STDOUT, "%d%s", next_term, next_term > 0 ? ",\n" : "");
        t_1 = t_2;
        t_2 = next_term;
        next_term = t_1 + t_2;
        if (next_term < 0) {
            return;
        }
    }
}
