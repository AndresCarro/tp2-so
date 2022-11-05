#include <processes.h>
#include <types.h>
#include <syslib.h>
#include <syscalls.h>

void mem(int argc, char * argv[]) {
    MemInfo * info = mem_info();
    fprintf(STDOUT, "%-20s%-15s%-20s%-15s\n", "MEMORY ALGORITHM", "TOTAL MEMORY", "OCCUPIED MEMORY", "FRAGMENTS");
    fprintf(STDOUT, "%-20s%-15d%-20d%-15d\n", info->mem_algorithm, info->memory_total, info->memory_occupied, info->memory_free, info->memory_frags);
    free((uint64_t) info->mem_algorithm);
    free((uint64_t) info);
}