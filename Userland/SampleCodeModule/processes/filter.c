// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>
#include <types.h>
#include <syslib.h>

char is_vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U'; 
}

void filter(int argc, char * argv[]) {
    int i = 0, c = get_char();
    while (c != EOF){
        if (!is_vowel(c)) {
            if(c == '\b' && i > 0) {
                i--;
                put_char(c);
            } else if(c == '\b' && i == 0 ) {

            }else{
                i++;
                put_char(c);
            }
        } else {
            put_char(' ');
        }
        c = get_char();
    }
    put_char('\n');
}