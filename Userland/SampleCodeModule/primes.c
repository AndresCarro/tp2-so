#include <processes.h>
#include <types.h>
#include <syslib.h>

int is_prime(int n) {
	int i;
	for(i = 2; i <= n / 2; i++) {
		if (n % i == 0) {
			return 0;
        }
	}
	return 1;
}

void primes(int argc, char * argv[]) {
    int i = 2;
    fprintf(STDOUT, "Prime numbers: \n");
    while (1) {
        if (i < 0) {
            return;
        }
        if (is_prime(i)) {
            fprintf(STDOUT, "%d,\n", i);
        }
        i++;
    }
}