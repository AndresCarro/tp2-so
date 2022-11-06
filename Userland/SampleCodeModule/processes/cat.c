// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>
#include <types.h>
#include <syslib.h>

void cat(int argc, char * argv[]) {
    int i = 0, c = get_char();
    while (c != EOF) {
        if (c == '\b' && i > 0) {
            i--;
            put_char(c);
        } else if(c == '\b' && i == 0 ) {
        
        } else {
            i++;
            put_char(c);
        }
        c = get_char();
    }
    put_char('\n');
}