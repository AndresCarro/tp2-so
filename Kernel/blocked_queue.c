#include <blocked_queue.h>

// Esto es lo que usa un proceso para determinar los procesos bloqueados por el mismo
// Ejemplo: lo usaria el semaforo para saber quienes estan bloqueados esperando al semaforo
BlockedQueueADT new_blocked_queue() {
    BlockedQueueADT queue = memory_manager_alloc(sizeof(BlockedQueueCDT));
    if(queue == NULL) {
        return NULL;
    }
    queue->first = NULL;
    queue->last = NULL;
    queue->qty = 0;
    return queue;
}

int is_empty(BlockedQueueADT queue) {
    if (queue == NULL) {
        return -1;
    }
    return queue->qty == 0;
}

pid_t dequeue_pid(BlockedQueueADT queue) {
    if(queue == NULL) {
        return -1;
    }
    BlockedNode * first = queue->first;
    if(first == NULL) {
        return -1;
    }
    queue->qty--;
    queue->first = first->next;
    pid_t ans = first->pid;
    memory_manager_free(first);
    return ans;
}

void enqueue_pid(BlockedQueueADT queue, pid_t pid) {
    if(queue == NULL) {
        return -1;
    }
    BlockedNode * new_node = memory_manager_alloc(sizeof(BlockedNode));
    new_node->pid = pid;
    new_node->next = NULL;
    if(queue->first == NULL) {
        queue->first = new_node;
        queue->last = new_node;
    } else {
        queue->last->next = new_node;
        queue->last = new_node;
    }
    queue->qty++;
}

unsigned int get_blocked_qty(BlockedQueueADT queue) {
    return queue->qty;
}

void free_queue(BlockedQueueADT queue) {
    if (queue == NULL) {
        return;
    }

    while (queue->first != NULL) {
        BlockedNode * aux = queue->first;
        queue->first = queue->first->next;
        memory_manager_free(aux);
    }
    memory_manager_free(queue);
}