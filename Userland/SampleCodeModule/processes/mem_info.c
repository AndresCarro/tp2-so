#include <processes.h>
#include <types.h>
#include <syslib.h>
#include <syscalls.h>

void mem(int argc, char * argv[]) {
    MemInfo * info = mem_info();
    fprintf(STDOUT, "MEMORY INFORMATION\n");
    fprintf(STDOUT, "Memory Algorithm: %s\nTotal Memory: %d\nOccupied Memory: %d\nFree Memory: %d\nFragments: %d\n", info->mem_algorithm, info->memory_total, info->memory_occupied, info->memory_free, info->memory_frags);
    free((uint64_t) info->mem_algorithm);
    free((uint64_t) info);
}