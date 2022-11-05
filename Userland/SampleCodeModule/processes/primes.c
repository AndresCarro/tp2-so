// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>
#include <types.h>
#include <syslib.h>

int is_prime(long n) {
	long i;
	for (i = 2; i <= n / 2; i++) {
		if (n % i == 0) {
			return 0;
        }
	}
	return 1;
}

void primes(int argc, char * argv[]) {
    long i = 2;
    fprintf(STDOUT, "Prime numbers: \n");
    while (1) {
        if (is_prime(i)) {
            fprintf(STDOUT, "%d,\n", i);
        }
        i = i + 1;
        if (i < 0) {
            return;
        }
    }
}