#include <wc.h>

void wc(){
    char * text;
    int size_read = gets(text);
    if(size_read == 0){
        puts("Hay 0 lineas");
        return;
    }
    int j=0,lines=1;
    for(int i = 0;i < size_read;i++){
        if(text[i] == '\n' || j == 80){
            lines++;
            j=0;
        }
    }
    fprintf(STDOUT,"Hay %d lineas\n",lines);
}