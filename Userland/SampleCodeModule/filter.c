#include <filter.h>

void filter(){
    int i = 0, c = get_char();
    while (c != EOF){
        if(!is_vowel(c)){
            if(c == '\b' && i > 0){
                i--;
                put_char(c);
            }else if(c == '\b' && i == 0 ){
                //no hago nada
            }else{
                i++;
                put_char(c);
            }
        } else {
            put_char(' ');
        }
        c = get_char();
    }
}

char is_vowel(char c){
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
     c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U'; 
}