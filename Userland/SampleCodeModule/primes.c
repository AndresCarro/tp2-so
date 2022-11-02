#include <primes.h>

//Devuelve 1 si es primo, 0 si no lo es.
int is_prime(int n){
	int i;
	for(i=2;i<=n/2;i++)
	{
		if(n%i==0)
			return 0;
	}
	return 1;
}

//Ciclo infinito que imprime numeros primos
void print_prime(){
    char num[30];
    int i=2;
    puts("Prime numbers: \n");
    while(1){
        if(is_prime(i)){
            if(num<0){//por si se pasa del max integer
                return;
            }
            itoa(i,num);
            puts(num);
            puts(",\n");
        }
        i++;
        halt();
    }
}