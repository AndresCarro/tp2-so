#include <test_util.h>

extern char bss;
extern char end_of_binary;

int main();

int _start() {
	memset(&bss, 0, &end_of_binary - &bss);
	return main();
}
