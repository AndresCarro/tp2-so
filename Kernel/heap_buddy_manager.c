#include <memory_manager.h>

#define FREE 0
#define TAKEN 1

#define ALLOC 1
#define DEALLOC -1

/* Maximum level is 25, this means 2^25 = 32MB*/
#define MAX_LEVEL 25
/* Minimun level is 8, this means 2^8 = 256B*/
#define MIN_LEVEL 8
/* La minima cantidad de memoria es 33MB */
#define MIN_MEMORY_SIZE (33 * 1024 * 1024)
/* La cantidad de memoria del heap es 32MB */
#define HEAP_MEMORY_SIZE (32 * 1024 * 1024)
/* Cantidad máxima de nodos */
#define MAX_NODES 262143


typedef struct {
    /* Whether this node is in use by a malloc. */
    unsigned int status;

    /* The amount of subnodes (including this node) that are marked as occupied. */
    unsigned int occupied_subnodes;
} TNode;

TNode * nodes;

uint64_t memory_start;
uint64_t total_memory;
uint64_t used_memory;
uint64_t memory_fragments;

static int get_max_posible_level(void* ptr) {
    int relativeAddress = ptr - memory_start;
    int level = MAX_LEVEL;
    int pow_of_two = 1 << MAX_LEVEL;
    while ((relativeAddress % pow_of_two) != 0) {
        level--;
        pow_of_two = pow_of_two >> 1;
    }
    return level;
}

static unsigned int get_level(uint64_t size) {
    unsigned int level = 0;
    /* 
        Tengo que restarle 1 a size para poder obtener el level correctamente 
        Ejemplo: size = 8MB = 2^13 y en binario 10000000000000 que es un 1 y trece 0.
        size es cero cuando muevo el 1 catorce veces y ese no es el level porque 2^14 != 2^13
        entonces lo que hago es hacer size-- para obtener el 2^13 correspondiente.
    */
    size--;
    while (size != 0) {
        size >>= 1;
        level++;
    }
    return level;
}

static unsigned int get_first_index_of_level(unsigned int level) {
    return (1 << (MAX_LEVEL - level)) - 1;
}

static unsigned int get_first_free_block_by_level(unsigned int level) {
    int index = get_first_index_of_level(level);
    int next_level_index = get_first_index_of_level(level - 1);

    while (index < next_level_index && (nodes[index].status || nodes[index].occupied_subnodes)) {
        index++;
    }

    if(index == next_level_index) {
        return -1;
    }

    return index;
}

static unsigned int get_right_children(unsigned int index) {
    return (index << 1) + 1;
}

static unsigned int get_left_children(unsigned int index) {
    return (index << 1) + 2;
}

static void set_status(unsigned int index, unsigned int status) {
    
    if (index < 0 || index >= MAX_NODES) {
        return;
    }

    nodes[index].status = status;
    /* Tengo que actualizar a los hijos */
    set_status(get_right_children(index), TAKEN);
    set_status(get_left_children(index), TAKEN);
}

static void update_parent(unsigned int index, unsigned int operation) {
    if (index < 0 || index >= MAX_NODES) {
        return;
    }
    nodes[index].occupied_subnodes += operation;

    /* Si soy el primer nodo no tengo que actualizar ningun padre porque soy padre de todos*/
    while (index != 0) {
        if (index % 2) {
            index = ((index + 1) >> 1) - 1;
        } else {
            index = (index + 1) >> 1;
        }
        nodes[index].occupied_subnodes += operation;
    }
}

static void * address(unsigned int index, unsigned int level) {
    return memory_start + (index - get_first_index_of_level(level)) * (1 << level);
}

static int get_start_searching_index(void* ptr, int maxLevel) {
    int relativeAddress = ptr - memory_start;
    return get_first_index_of_level(maxLevel) + (relativeAddress / (1 << maxLevel));
}

static int search_node(int index, int* level, void* ptr) {
    if (*level < MIN_LEVEL)
        return -1;

    if (!nodes[index].status) {
        (*level)--;
        int rightChildIndex = get_right_children(index);
        void* current_address = address(rightChildIndex, *level);
        return search_node(ptr < current_address ? get_left_children(index) : rightChildIndex, level, ptr);
    }

    return ptr == address(index, *level) ? index : -1;
}

//Crea la lista de memoria, recibe la direcc de memoria donde arranca el heap y el size del mismo.
void memory_manager_start(void * start_address, uint64_t size_heap) {
    memory_start = start_address;
    // if (size_heap < MIN_MEMORY_SIZE)
    //     return;

    // Allocate an array to represent a complete binary tree
    nodes = start_address;
    size_t nodesMemorySize = sizeof(TNode) * MAX_NODES;

    memory_start += nodesMemorySize;

    total_memory = HEAP_MEMORY_SIZE;
    used_memory = 0;
    memory_fragments = 0;

    // Set all nodes as free
    for (int i = 0; i < MAX_NODES; ++i) {
        nodes[i].status = FREE;
        nodes[i].occupied_subnodes = 0;
    }
}

//Toma la cantidad necesaria de nodos de la lista para alocar el size requested.
void * memory_manager_alloc(uint64_t size) {
    unsigned int level;

    if (size > HEAP_MEMORY_SIZE || size <= 0 || (level = get_level(size)) > MAX_LEVEL) {
        return NULL;
    }

    if (level < MIN_LEVEL) {
        return;
    }

    /* Lo buscamos en el array */
    int index = get_first_free_block_by_level(level);

    if (index < 0) {
        return NULL;
    }

    update_parent(index, ALLOC);
    set_status(index, TAKEN);

    memory_fragments++;
    used_memory += (1 << level);

    return address(index, level);
}

/*Libera la memoria y agrega un/unos nodo/s a la lista de memoria libre. Que si tienen 
nodos contiguos libres los junta.*/
void memory_manager_free(void * ptr) {
    if (ptr == NULL) {
        return;
    }

    if(ptr < memory_start || ptr >= (memory_start + HEAP_MEMORY_SIZE)) {
        return;
    }

    int level = get_max_posible_level(ptr);
    int index = search_node(get_start_searching_index(ptr, level), &level, ptr);

    if (index < 0) {
        return;
    }

    update_parent(index, DEALLOC);
    set_status(index, FREE);

    memory_fragments--;
    used_memory -= (1 << level);
}

MemInfo * mem_info() {
    MemInfo * info = memory_manager_alloc(sizeof(MemInfo));
    info->memory_total = total_memory;
    info->memory_occupied = used_memory;
    info->memory_free = total_memory - used_memory;
    info->memory_frags = memory_fragments;
    return info;
}

