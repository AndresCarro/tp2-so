#include <stdint.h>
#include <syslib.h>
#include <syscalls.h>
#include <types.h>

#define MINOR_WAIT 30000000 // Change this value to prevent a process from flooding the screen

// Random
static uint32_t m_z = 362436069;
static uint32_t m_w = 521288629;

uint32_t get_uint() {
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

uint32_t get_uniform(uint32_t max) {
    uint32_t u = get_uint();
    return (u + 1.0) * 2.328306435454494e-10 * max;
}

//Memory
void * memset(void * destination, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

uint8_t memcheck(void *start, uint8_t value, uint32_t size) {
    uint8_t *p = (uint8_t *) start;
    uint32_t i;

    for (i = 0; i < size; i++, p++)
        if (*p != value)
            return 0;

    return 1;
}

// Processes
void busy_wait(uint64_t n) {
    uint64_t i;
    for (i = 0; i < n; i++);
}

void endless_loop() {
    while (1);
}

void endless_loop_print(int argc, char * argv[]) {
    pid_t pid = getpid();
    while (1) {
        fprintf(STDOUT, "%d ",pid);
        busy_wait(MINOR_WAIT);
    }
}