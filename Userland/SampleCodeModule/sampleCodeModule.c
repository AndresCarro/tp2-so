// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <bash.h>
#include <syslib.h>
#include <types.h>
#include <syscalls.h>

int main() {
    char welcome[] = "\n                                    turtleOS\n"
                     "       By Andres Carro Wetzel, Alejo Flores Lucey & Nehuen Gabriel Llanos\n"
                     "\n\n                             PRESS ANY KEY TO START\n";
    fprintf(STDOUT, "\n\n\n\n%s%s", TURTLE, welcome);
    get_char();
    clear();
	bash();
	return 0x600DB7E;
}