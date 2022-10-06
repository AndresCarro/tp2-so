#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>
#include <stdint.h>

typedef struct{
    uint64_t memory_free;
    uint64_t memory_occupied;
    uint64_t memory_total;
    uint64_t memory_nodes; // Mientras mas nodos mas fragmentacion.
}Memory_State;

//Crea la lista de memoria, recibe la direcc de memoria donde arranca el heap y el size del mismo.
void memory_manager_start(void * start_address, uint64_t size_heap);

//Toma la cantidad necesaria de nodos de la lista para alocar el size requested.
void * memory_manager_alloc( uint64_t size );

/*Libera la memoria y agrega un/unos nodo/s a la lista de memoria libre. Que si tienen 
nodos contiguos libres los junta.*/
void memory_manager_free(void * ptr);

//Devuelve a traves de Memory_State informacion sobre la memoria.
void memory_manager_get_state(Memory_State * memory_state);

#endif