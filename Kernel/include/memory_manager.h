#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>
#include <kernel_types.h>

void memory_manager_start(void * start_address, uint64_t size_heap);
void * memory_manager_alloc( uint64_t size );
void memory_manager_free(void * ptr);

MemInfo * mem_info();

#endif