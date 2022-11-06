#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void * memmove(void * dest, void * src, unsigned int n);
int strcmp(const char * p1, const char * p2);
char * strcpy(const char * str);
unsigned int strlen(const char *str);

extern uint8_t get_seconds();
extern uint8_t get_minutes();
extern uint8_t get_hours();
extern uint8_t get_day();
extern uint8_t get_month();
extern uint8_t get_year();
extern uint8_t read_port(uint8_t key);
extern char * cpu_vendor(char * result);

#endif