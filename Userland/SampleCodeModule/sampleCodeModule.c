/* sampleCodeModule.c */

#include <bash.h>

char * v = (char*)0xB8000 + 79 * 2;

int main() {
	bash();
	return 0x600DB7E;
}