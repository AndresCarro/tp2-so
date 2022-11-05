#ifndef SYSLIB_H
#define SYSLIB_H

#include <stdint.h>

#define WAIT_TIME 1947324

char ** strtok(char * str, char delim, int * qty);
unsigned int strlen(char * s);
int strcmp(char * p1, char * p2);
int atoi(char * str);
void itoa(int n, char s[]);
uint32_t uint_to_base(uint64_t value, char * buffer, uint32_t base);

int is_num(char c);

void fprintf(int fd, char * str, ...);
int put_char(char);
char get_char();
int gets(char * s);

unsigned int char_belongs(char * s,char c);
int contains_string(char * p1, char * p2);

#endif