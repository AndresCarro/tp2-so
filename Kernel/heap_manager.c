#include <memory_manager.h>

uint64_t total_memory;
uint64_t used_memory;
uint64_t memory_node_count;

typedef struct{
    uint64_t size;
    uint64_t leftover;
    (List_CDT *) next;
    (List_CDT *) previous;
}Memory_Node_CDT;

typedef Memory_Node_CDT * Memory_Node_ADT;

Memory_Node_ADT head = NULL;

//Crea la lista de memoria, recibe la direcc de memoria donde arranca el heap y el size del mismo.
void memory_manager_start( (void *) start_address, uint64_t size_heap){
    total_memory = size_heap;
    used_memory = sizeof( Memory_Node_CDT );
    memory_node_count = 1;
    head = start_address;
    head->size = 0;
    head->leftover = total_memory - used_memory;
    head->next = NULL;
    head->previous = NULL;
}

//Toma la cantidad necesaria de nodos de la lista para alocar el size requested.
void * memory_manager_alloc( uint64_t size ){
    if(head == NULL || size == 0 || total_memory - used_memory < size){
        return NULL;
    }
    Memory_Node_ADT current = head;
    while(current->leftover < size + sizeof(Memory_Node_CDT)){
        if(current == NULL){
            return NULL;
        }
        current = current->next;
    }
    if(current->size == 0){
        current->size = size;
        current->leftover -= size;
        used_memory += size;
        return ( void * ) current + sizeof(Memory_Node_CDT);
    }else{
        Memory_Node_ADT new_node = current + current->size + sizeof(Memory_Node_CDT);
        new_node->size = size;
        new_node->leftover = current->leftover - size - sizeof(Memory_Node_CDT);
        current->leftover = 0;
        new_node->next = current->next;
        new_node->previous = current;
        if(current->next != NULL){
            current->next->previous = new_node;
        }
        current->next = new_node;
        used_memory += size + sizeof(Memory_Node_CDT);
        memory_node_count++;
        return ( void * ) new_node + sizeof(Memory_Node_CDT);
    }
}

/*Libera la memoria y agrega un/unos nodo/s a la lista de memoria libre. Que si tienen 
nodos contiguos libres los junta.*/
void memory_manager_free( (void *)ptr ){
    
}

//Devuelve a traves de Memory_State informacion sobre la memoria.
void memory_manager_get_state((Memory_State *) memory_state){

}
