// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>
#include <types.h>
#include <syslib.h>
#include <syscalls.h>

void mem(int argc, char * argv[]) {
    MemInfo * info = mem_info();
    fprintf(STDOUT, "%-20s%-10s%-10s%-10s%-10s\n", "MEMORY ALGORITHM", "TOTAL", "OCCUPIED", "FREE", "FRAGMENTS");
    fprintf(STDOUT, "%-20s%-10d%-10d%-10d%-10d\n", info->mem_algorithm, info->memory_total, info->memory_occupied, info->memory_free, info->memory_frags);
    free((uint64_t) info->mem_algorithm);
    free((uint64_t) info);
}