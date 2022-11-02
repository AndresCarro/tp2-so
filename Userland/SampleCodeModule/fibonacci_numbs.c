#include <fibonacci_numbs.h>

void fibonacci_numbs(){
    char num[30];
    int t1 = 0, t2 = 1;
    long nextTerm = t1 + t2;
    puts("Fibonacci Series: 0, 1, ");
    while(1) {
        itoa(nextTerm,num);
        puts(num);
        puts(",");
        t1 = t2;
        t2 = nextTerm;
        nextTerm = t1 + t2;
        if(nextTerm<0){//por si se pasa del max integer
            puts("\b");
            return;
        }
        puts("\n");
    }
}
