#include <print_mem_info.h>

void print_mem_info() {
    MemInfo * info = mem_info();
    fprintf(STDOUT, "Total: %d, Occupied: %d, Free: %d, Fragments: %d\n", info->memory_total, info->memory_occupied, info->memory_free, info->memory_frags);
}