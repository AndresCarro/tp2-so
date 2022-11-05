// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef BUDDY_MM

#include <memory_manager.h>
#include <lib.h>

#define MEM_ALGORITHM "Buddy"

#define MAX_LEVEL 23
#define MIN_LEVEL 8

#define MIN_MEMORY_SIZE (9 * 1024 * 1024)
#define HEAP_MEMORY_SIZE (8 * 1024 * 1024)

#define MAX_NODES 65535

#define FREE 0
#define TAKEN 1
#define ALLOC 1
#define DEALLOC -1

uint64_t total_memory;
uint64_t used_memory;
uint64_t memeory_fragments;

typedef struct {
    unsigned int status;
    unsigned int occupied_children;
} Buddy;

Buddy * buddies;

void * memory_start;
void * heap_start;

void memory_manager_start(void * start_address, uint64_t size_heap) {
    if (size_heap < MIN_MEMORY_SIZE) {
        return;
    }
    
    memory_start = start_address;
    buddies = start_address;
    size_t buddie_memory_size = sizeof(Buddy) * MAX_NODES;
    heap_start = memory_start + buddie_memory_size;

    total_memory = HEAP_MEMORY_SIZE;
    used_memory = 0;
    memeory_fragments = 0;

    for (int i = 0; i < MAX_NODES; ++i) {
        buddies[i].status = FREE;
        buddies[i].occupied_children = 0;
    }
}

unsigned int get_level(uint64_t size) {
    unsigned int level = 0;
    size--;
    while (size != 0 ) {
        level++;
        size >>= 1;
    }
    return level;
}

unsigned int get_first_index_for_level(unsigned int level) {
    return (1 << (MAX_LEVEL - level)) - 1;
}

int get_free_index_for_level(unsigned int level) {
    unsigned int index = get_first_index_for_level(level);
    unsigned int index_prev_level = get_first_index_for_level(level - 1);

    while (index < index_prev_level && (buddies[index].status || buddies[index].occupied_children)) {
        index++;
    }

    if (index == index_prev_level) {
        return -1;
    }

    return index;
}

unsigned int get_left_child_index(unsigned int index) {
    return (index << 1) + 1;
}

unsigned int get_right_child_index(unsigned int index) {
    return (index << 1) + 2;
}

unsigned int get_parent_index(unsigned int index) {
    return ((index + 1) >> 1) - 1;
}

void update_parents_status(unsigned int index, unsigned int operation) {
    buddies[index].occupied_children += operation;

    while (index != 0) {
        index = get_parent_index(index);
        buddies[index].occupied_children += operation;
    }
}

void set_status(unsigned int index, unsigned int new_status) {
    if (index < 0 || index >= MAX_NODES){
        return;
    }

    buddies[index].status = new_status;
    set_status(get_left_child_index(index), new_status);
    set_status(get_right_child_index(index), new_status);
}

void * obtain_address(unsigned int index, unsigned int level) {
    void * return_address = heap_start;

    uint64_t buddy_diff_index = index - get_first_index_for_level(level);
    uint64_t buddy_size = (1 << level);

    return_address += (buddy_diff_index * buddy_size);

    return return_address;
}

void * memory_manager_alloc( uint64_t size ) {
    unsigned int level;
    if (size == 0 || size > HEAP_MEMORY_SIZE || (level = get_level(size)) > MAX_LEVEL) {
        return NULL;
    }

    if (level < MIN_LEVEL) {
        level = MIN_LEVEL;
    }

    int index = get_free_index_for_level(level);

    if (index < 0) {
        return NULL;
    }

    update_parents_status(index, ALLOC);
    set_status(index, TAKEN);

    memeory_fragments++;
    used_memory += (1 << level);

    return obtain_address(index, level);
}

int search_node(int index, int * level, void * ptr) {
    if (*level < MIN_LEVEL) {
        return -1;
    }

    if (!buddies[index].status) {
        (*level)--;
        int right_child_index = get_right_child_index(index);
        void * address = obtain_address(right_child_index, *level);
        return search_node(ptr < address ? get_left_child_index(index) : right_child_index, level, ptr);
    }

    if (ptr == obtain_address(index, *level)) {
        return index;
    }
    return -1;
}


int get_max_posible_level(void * ptr) {
    int relative_address = ptr - heap_start;
    int level = MAX_LEVEL;
    int pow_of_two = 1 << MAX_LEVEL;

    while ((relative_address % pow_of_two) != 0) {
        level--;
        pow_of_two = pow_of_two >> 1;
    }
    return level;
}

int get_start_searching_index(void * ptr, int max_level) {
    int relative_address = ptr - heap_start;
    return get_first_index_for_level(max_level) + (relative_address / (1 << max_level));
}

void memory_manager_free(void * ptr) {
    if (ptr == NULL || ptr < heap_start || ptr >= (heap_start + HEAP_MEMORY_SIZE)) {
        return;
    }

    int level = get_max_posible_level(ptr);
    int index = search_node(get_start_searching_index(ptr, level), &level, ptr);

    if (index < 0) {
        return;
    }

    update_parents_status(index, DEALLOC);
    set_status(index, FREE);

    memeory_fragments--;
    used_memory -= (1 << level);
}

MemInfo * mem_info() {
    MemInfo * info = memory_manager_alloc(sizeof(MemInfo));
    info->mem_algorithm = strcpy(MEM_ALGORITHM);
    info->memory_total = total_memory;
    info->memory_occupied = used_memory;
    info->memory_free = total_memory - used_memory;
    info->memory_frags = memeory_fragments;
    return info;
}

#endif