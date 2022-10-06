#include <scheduler2.h>


Queue active = NULL;
Queue expired = NULL;


unsigned int process_count = 0;
priority_t priorities[TOT_PRIORITIES] = {0, 9, 8, 7, 6, 5, 4, 3, 2, 1};

pid_t create_process(uint64_t rip, int argc, char * argv[]) {
    Node * new_process = memory_manager_alloc(sizeof(Node));
    new_process->process.pid = ++process_count;
    new_process->process.priority = DEF_PRIORITY;
    new_process->process.quantums_left = priorities[DEF_PRIORITY];
    new_process->process.status = READY;

    // TODO DESALOJAR PROCESO
    if(active == NULL) {
        new_process->next = NULL;
        active = new_process;
    } else {
        Node * current = active;
        Node * previous = NULL;
        // Si el numero del que quiero insertar es mayor que el current entonces tengo que insertarlo despues
        while(current->next != NULL || new_process->process.priority >= current->process.priority) {
            previous = current;
            current = current->next;
        }
        if(current->next == NULL && new_process->process.priority >= current->process.priority) {
            new_process->next = NULL;
            current->next = new_process;
        } else {
            new_process->next = current;
            if(previous != NULL) {
                previous->next = new_process;
            } else {
                active = new_process;
            }
        }
    }
    // TODO ASSEMBLER TO CREATE STACK

    return new_process->process.pid;
}

// El rsp es el rsp del proceso que se estaba corriendo. Donde quedaron los registros
void context_switch(uint64_t rsp) {
    Node * current_process = active;
    current_process->process.rsp = rsp;

    active = current_process->next;
    Node * current_expired = expired;

    if(current_expired == NULL) {
        // SI ENTRO ACA ENTONCES EXPIRED ES VACIO
        // EXPIRED NO TIENE NADA 
        expired = current_process;
        expired->next = NULL;
    } else {
        while(current_expired->next != NULL) {
            current_expired = current_expired->next;
        }
        current_expired->next = current_process;
        current_process->next = NULL;
    }
    // CHEQUEAR QUE ACTIVE ES NULL
    if(active == NULL) {
        active = expired;
        expired = NULL;
    }
    // llamar a assembler con el rsp de active
}


