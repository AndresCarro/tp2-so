// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef BUDDY_MM

#include <memory_manager.h>
#include <lib.h>

#define MEM_ALGORITHM "Heap"

uint64_t total_memory;
uint64_t used_memory;
uint64_t memory_node_count;

typedef struct memorynodecdt {
    uint64_t size;
    uint64_t leftover;
    struct memorynodecdt * next;
    struct memorynodecdt * previous;
} MemoryNodeCDT;

typedef MemoryNodeCDT * MemoryNodeADT;

MemoryNodeADT head = NULL;

void memory_manager_start(void * start_address, uint64_t size_heap) {
    total_memory = size_heap;
    used_memory = sizeof(MemoryNodeCDT);
    memory_node_count = 1;
    head = start_address;
    head->size = 0;
    head->leftover = total_memory - used_memory;
    head->next = NULL;
    head->previous = NULL;
}

void * memory_manager_alloc(uint64_t size) {
    if (head == NULL || size == 0 || total_memory - used_memory < size) {
        return NULL;
    }
    MemoryNodeADT current = head;
    while (current->leftover < size + sizeof(MemoryNodeCDT)) {
        if (current == NULL) {
            return NULL;
        }
        current = current->next;
    }
    if (current->size == 0) {
        current->size = size;
        current->leftover -= size;
        used_memory += size;
        return (void *) current + sizeof(MemoryNodeCDT);
    } else {
        MemoryNodeADT new_node = (void *) ((uint64_t) current + current->size + sizeof(MemoryNodeCDT));
        new_node->size = size;
        new_node->leftover = current->leftover - size - sizeof(MemoryNodeCDT);
        current->leftover = 0;
        new_node->next = current->next;
        new_node->previous = current;
        if (current->next != NULL) {
            current->next->previous = new_node;
        }
        current->next = new_node;
        used_memory += size + sizeof(MemoryNodeCDT);
        memory_node_count++;
        return (void *) new_node + sizeof(MemoryNodeCDT);
    }
}

void memory_manager_free(void * ptr) {
    if (ptr == NULL) {
        return;
    }
    MemoryNodeADT current = ptr - sizeof(MemoryNodeCDT);
    if (current->previous == NULL) {
        current->leftover += current->size;
        used_memory -= current->size;
        current->size = 0;
    } else {
        MemoryNodeADT prev = current->previous;
        prev->next = current->next;
        if (current->next != NULL) {
            current->next->previous = prev;
        }
        prev->leftover += current->size + current->leftover + sizeof(MemoryNodeCDT);
        used_memory -= current->size + sizeof(MemoryNodeCDT);
        memory_node_count--;
    }
}

MemInfo * mem_info() {
    MemInfo * info = memory_manager_alloc(sizeof(MemInfo));
    if (info == NULL) {
        return NULL;
    }
    info->mem_algorithm = strcpy(MEM_ALGORITHM);
    info->memory_total = total_memory;
    info->memory_occupied = used_memory;
    info->memory_free = total_memory - used_memory;
    info->memory_frags = memory_node_count;
    return info;
}

#endif