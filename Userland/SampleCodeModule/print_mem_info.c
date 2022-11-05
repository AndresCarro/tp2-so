#include <processes.h>
#include <types.h>

void mem() {
    MemInfo * info = mem_info();
    fprintf(STDOUT, "Total: %d, Occupied: %d, Free: %d, Fragments: %d\n", info->memory_total, info->memory_occupied, info->memory_free, info->memory_frags);
}