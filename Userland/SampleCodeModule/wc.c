#include <wc.h>

void prueba(){}

void wc(){
    int lines=0;
    char c,previous=0;
    while( (c = get_char()) != EOF ){
        if(c == '\n'){
            prueba();
            lines++;
        }
        put_char(c);
        previous = c;
    }
    if( previous != 0 && lines == 0){
        lines = 1;
    }else if(previous != '\n'){
        lines++;
    }
    fprintf(STDOUT,"Hay %d lineas\n",lines);
}