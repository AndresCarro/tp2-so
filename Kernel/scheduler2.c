#include <scheduler2.h>
#include <interrupts.h>
#include <naiveConsole.h>

Queue active = NULL;
Queue expired = NULL;

extern uint64_t load_process(uint64_t rip, uint64_t rsp, uint64_t argc, uint64_t argv);

int process_count = -1;
// Esta variable lo que hace es contar la cantidad de procesos listos en ambas listas, tanto la de
// activos como la de expirados
unsigned int process_ready_count = 0;
unsigned char something_running = 0;
priority_t priorities[TOT_PRIORITIES] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
pid_t dummy_process_pid = NULL;

void dummy_process() {
    while (1) {
        ncPrint("A");
        _hlt();
    }
}

void scheduler_init() {
    dummy_process_pid = create_process(dummy_process, 0, NULL);
}

// Obtener el proceso de la lista de activos o de la lista de expirados y llevar ese puntero del PCB a la lista de bloqueados.
// Devuelve 1 si lo bloqueo, sino devuelve 0
char block_process(pid_t process_pid) {
    Node * current = active;
    Node * previous = NULL;
    char found = 0;

    // Buscamos en la lista de activos
    while(!found && current != NULL) {
        if(current->process.pid == process_pid) {
            found = 1;
            // Primer elemento
            if(previous == NULL) {
                
                active = current->next;
            } else {
                previous->next = current->next;
            }
            current->process.status = BLOCKED;
        } else {
            previous = current;
            current = current->next;
        }
    }
    // Si no lo encontre en la de activos entonces lo busco en la expirados
    current = expired;
    previous = NULL;
    while(!found && current != NULL) {
        if(current->process.pid == process_pid) {
            found = 1;
            // Primer elemento
            if(previous == NULL) {
                expired = current->next;
            } else {
                previous->next = current->next;
            }
            current->process.status = BLOCKED;
        } else {
            previous = current;
            current = current->next;
        }
    }
    // Llamamos al rsp del que se ejecuto ahora
    if(found) {
        process_ready_count--;
        _int20h();
    }
}

pid_t create_process(uint64_t rip, int argc, char * argv[]) {
    Node * new_process = memory_manager_alloc(sizeof(Node));
    new_process->process.pid = process_count++;
    new_process->process.priority = DEF_PRIORITY;
    new_process->process.quantums_left = priorities[DEF_PRIORITY];
    new_process->process.status = READY;
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
    new_process->process.stack_base = rsp;
    // Este new_rsp es el que tengo que guardar en el pcb porque es donde esta
    // guardado el stack que me va a permitir correr nuevamente el proceso en el contexto
    // en el que me encontraba.
    uint64_t new_rsp = load_process(rip, rsp + 4 * 1024, argc, argv);
    new_process->process.rsp = new_rsp;
    
    if (active == NULL) {
        new_process->next = NULL;
        active = new_process;
    } else {
        if(expired == NULL) {
            new_process->next = NULL;
            expired = new_process;
        } else {
            Node * current = expired;
            Node * previous = NULL;
            // Si el numero del que quiero insertar es mayor que el current entonces tengo que insertarlo despues
            // new_process -> 2 y current -> 1. La 1 es mejor que la 2. El menor numero gana
            while(current->next != NULL && new_process->process.priority >= current->process.priority) {
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
                    expired = new_process;
                }
            }
        }
    }
    process_ready_count++;
    return new_process->process.pid;
}

// El rsp es el rsp del proceso que se estaba corriendo. Donde quedaron los registros
uint64_t context_switch(uint64_t rsp) {
    Node * current_process = active;
    // Si no empezo el userland entonces lo unico que hiciste fue iniciar el kernel y cargar el userland a la lista de activos
    // Si no tengo nada corriendo porque el proceso que estaba corriendo terminó, entonces tengo que poner a correr
    // algun proceso en la lista de activos o expirados.
    if (!something_running) {
        something_running = 1;
        // Si current_process == NULL entonces en la lista de activos no tengo nada y por ende, tengo
        // que copiar la de expirados a la de activos y tambien actualizar a current_process porque luego
        // lo retorno
        if(current_process == NULL) {
            active = expired;
            current_process = active;
            expired = NULL;
        }
        // Si current_process != NULL entonces retorno ese mismo elemento porque es el siguiente a correrse.
        //TODO current_process != NULL pero tiene status BLOCKED
        return current_process->process.rsp;
    }
    // Este rsp es del proceso que estaba corriendo hasta el momento
    current_process->process.rsp = rsp;

    // Si el current_process no esta bloqueado y el proceso le sobran quantums entonces decremento en uno
    // los quantums y retorno el rsp
    if(current_process->process.status != BLOCKED && current_process->process.quantums_left > 0) {
        current_process->process.quantums_left--;
        return rsp;
    }
    current_process->process.quantums_left = priorities[current_process->process.priority];
    // En este caso el current_process se quedo sin quantums.
    // Active es ahora el siguiente proceso en la lista.
    active = current_process->next;
    // Si es igual a cero tengo que poner a correr el dummy
    if(process_ready_count == 0) {
        char dummy_found = 0;
        Node * current = active;
        Node * previous = NULL;
        while(current != NULL && !dummy_found && current->process.pid != dummy_process_pid) {
            previous = current;
            current = current->next;
        }
        if(dummy_found) {
            if(previous != NULL) {
                previous->next = current->next;
                current->next = active;
                active = current;
            }
            return active->process.rsp;
        }
        current = expired;
        previous = NULL;
        while(current != NULL && !dummy_found && current->process.pid != dummy_process_pid) {
            previous = current;
            current = current->next;
        }
        // Expired en este caso nunca va a ser NULL porque como minimo tiene a dummy process
        if(previous == NULL) {
            expired = expired->next;
        } else {
            previous->next = current->next;
        }
        current->next = active;
        active = current;
        return active->process.rsp;
    }    

    char found_next_to_run = 0;

    while(!found_next_to_run) {
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
        found_next_to_run = active->process.status != BLOCKED;
        current_process = active;
    }
    return active->process.rsp;
}

int terminate_process(int return_value){
    Node * current_process = active;
    active = current_process->next;
    process_ready_count--;
    memory_manager_free(current_process->process.stack_base);
    memory_manager_free(current_process);
    something_running = 0;
    return return_value;
}

// int change_priority(int priority_value){
//     if(priority_value < 0 && priority_value > 8){//Fuera del rango de priorities
//         return -1;
//     }
//     active->priority = priority_value;
    
// }