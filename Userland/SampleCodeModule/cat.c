#include <cat.h>

void cat(){
    int i = 0, c = get_char();
    while (c != EOF){
        if(c == '\b' && i > 0){
            i--;
            put_char(c);
        }else if(c == '\b' && i == 0 ){
            //no hago nada
        }else{
            i++;
            put_char(c);
        }
        c = get_char();
    }
}