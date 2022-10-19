#include <scheduler2.h>


Queue active = NULL;
Queue expired = NULL;


unsigned int process_count = 0;
priority_t priorities[TOT_PRIORITIES] = {9, 8, 7, 6, 5, 4, 3, 2, 1};

pid_t create_process(uint64_t rip, int argc, char * argv[]) {
    Node * new_process = memory_manager_alloc(sizeof(Node));
    new_process->process.pid = ++process_count;
    new_process->process.priority = DEF_PRIORITY;
    new_process->process.quantums_left = priorities[DEF_PRIORITY];
    new_process->process.status = READY;

    // TODO ASSEMBLER TO CREATE STACK
    // ; Creamos el stack "simulado" del proceso para que el scheduler
    // ; pueda tomar el programa y correrlo
    // ; rdi -> entryPoint, el puntero a funcion rip
    // ; rsi -> rsp
    // ; rdx -> argc
    // ; rcx -> argv
    uint64_t rsp = memory_manager_alloc(4*1024);
    // En caso que el memory-manager devuelve NULL entonces debemos retornar -1
    // para avisar que no podemos albergar otro proceso.
    if(rsp == NULL) {
        return -1;
    }

    // Este new_rsp es el que tengo que guardar en el pcb porque es donde esta
    // guardado el stack que me va a permitir correr nuevamente el proceso en el contexto
    // en el que me encontraba.
    uint64_t new_rsp = load_process(rip, rsp, argc, argv);
    new_process->process.rsp = new_rsp;

    if(active == NULL) {
        new_process->next = NULL;
        active = new_process;
    } else {
        Node * current = active;
        Node * previous = NULL;
        // Si el numero del que quiero insertar es mayor que el current entonces tengo que insertarlo despues
        // new_process -> 2 y current -> 1. La 1 es mejor que la 2. El menor numero gana
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
    // TODO DESALOJAR PROCESO

    return new_process->process.pid;
}

// El rsp es el rsp del proceso que se estaba corriendo. Donde quedaron los registros
uint64_t context_switch(uint64_t rsp) {
    Node * current_process = active;
    current_process->process.rsp = rsp;

    if(current_process->process.quantums_left > 0) {
        current_process->process.quantums_left--;
        return rsp;
    }

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
    return active->process.rsp;
}


