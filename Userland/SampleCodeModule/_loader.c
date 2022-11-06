// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <test_util.h>

extern char bss;
extern char end_of_binary;

int main();

int _start() {
	memset(&bss, 0, &end_of_binary - &bss);
	return main();
}
