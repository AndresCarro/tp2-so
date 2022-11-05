#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <stdint.h>

uint32_t get_uint();
uint32_t get_uniform(uint32_t max);
void * memset(void * destination, int32_t c, uint64_t length);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
void busy_wait(uint64_t n);
void endless_loop();
void endless_loop_print(uint64_t wait);

#endif