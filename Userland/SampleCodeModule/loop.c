#include <loop.h>

void loop() {
    pid_t pid = getpid();
    int i=0;
    while(1){
        if(i % 433494437 == 0){
            fprintf(STDOUT,"Hello there! I'm %d :)\n",pid);
        }
        i++;
    }
}