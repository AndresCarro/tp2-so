// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>
#include <types.h>
#include <syslib.h>

void wc(int argc, char *argv[]) {
    int lines = 0;
    int c;
    char previous = 0;
    while ((c = get_char()) != EOF) {
        if (c == '\n') {
            lines++;
        }
        previous = c;
    }
    if (previous != 0 && lines == 0) {
        lines = 1;
    } else if (previous != '\n') {
        lines++;
    }
    fprintf(STDOUT,"%d lines were printed\n",lines);
}