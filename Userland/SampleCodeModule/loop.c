#include <processes.h>
#include <types.h>

void loop(int argc, char * argv[]) {
    pid_t pid = getpid();
    int i = 0;
    while (1) {
        if (i == 433494437) {
            fprintf(STDOUT, "Hello there! I'm %d :)\n",pid);
            i = 0;
        }
        i++;
    }
}