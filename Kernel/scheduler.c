#include <scheduler.h>
#include <interrupts.h>
#include <pipe.h>
#include <memory_manager.h>
#include <blocked_queue.h>
#include <lib.h>

Queue active = NULL;
Queue expired = NULL;
priority_t priorities[TOT_PRIORITIES] = {9, 8, 7, 6, 5, 4, 3, 2, 1};

extern uint64_t load_process(uint64_t rip, uint64_t rsp, uint64_t argc, uint64_t argv);
extern void _int20h();

int process_count = -1;
unsigned int process_ready_count = 0;
unsigned char something_running = 0;
pid_t dummy_process_pid = NULL;

void dummy_process() {
    while (1) {
        _hlt();
    }
}

void scheduler_init(Pipe * stdin) {
    char * name = "Kernel Task";
    char * argv[] = {name};
    dummy_process_pid = create_process((uint64_t) dummy_process, 1, argv);
    
    active->process.last_fd = 2;
    active->process.file_desciptors[0].mode = READ;
    active->process.file_desciptors[0].pipe = stdin;
    active->process.file_desciptors[1].mode = WRITE;
    active->process.file_desciptors[1].pipe = NULL;

    active->process.status = BLOCKED;
    process_ready_count--;
}

PCB * get_process(pid_t pid) {
    Node * current = active;
    while (current != NULL) {
        if (current->process.pid == pid) {
            return &(current->process);
        } else {
            current = current->next;
        }
    }
    current = expired;
    while (current != NULL) {
        if (current->process.pid == pid) {
            return &(current->process);
        } else {
            current = current->next;
        }
    }
    return NULL;
}

pid_t get_current_pid() {
    if (active != NULL) {
        return active->process.pid;
    }
    return -1;
}

int unblock_process(pid_t process_pid) {
    Node * current = active;
    char found = 0;

    while (!found && current != NULL) {
        if (current->process.pid == process_pid) {
            found = 1;
            current->process.status = READY;
        } else {
            current = current->next;
        }
    }
    current = expired;
    while (!found && current != NULL) {
        if (current->process.pid == process_pid) {
            found = 1;
            current->process.status = READY;
        } else {
            current = current->next;
        }
    }
    if (found) {
        process_ready_count++;
        return 0;
    }
    return -1;
}

int block_process(pid_t process_pid) {
    Node * current = active;
    char found = 0;

    while (!found && current != NULL) {
        if (current->process.pid == process_pid) {
            found = 1;
            current->process.status = BLOCKED;
        } else {
            current = current->next;
        }
    }
    current = expired;
    while (!found && current != NULL) {
        if (current->process.pid == process_pid) {
            found = 1;
            current->process.status = BLOCKED;
        } else {
            current = current->next;
        }
    }
    if (found) {
        process_ready_count--;
        _int20h();
        return 0;
    }
    return -1;
}

void copy_fd_table(fd_t src[], fd_t dest[], unsigned int qty) {
    for (int i = 0; i < qty; i++) {
        dest[i].mode = src[i].mode;
        dest[i].pipe = src[i].pipe;
        pipe_inherited(dest[i].pipe, dest[i].mode == WRITE ? 1 : 0);
    }
}

char ** copy_argv(int argc, char ** argv) {
    char ** new_argv = memory_manager_alloc(sizeof(char *) * argc);
    for (int i = 0; i < argc; i++) {
        new_argv[i] = strcpy(argv[i]);
    }
    return new_argv;
}

pid_t create_process(uint64_t rip, int argc, char * argv[]) {
    Node * new_process = memory_manager_alloc(sizeof(Node));
    new_process->process.pid = process_count++;
    new_process->process.priority = DEF_PRIORITY;
    new_process->process.quantums_left = priorities[DEF_PRIORITY];
    new_process->process.status = READY;
    new_process->process.blocked_queue = new_blocked_queue();
    new_process->process.new_priority = -1;
    new_process->process.argc = argc;
    new_process->process.argv = copy_argv(argc, argv);
    if (active != NULL) {
        new_process->process.last_fd = active->process.last_fd;
        copy_fd_table(active->process.file_desciptors, new_process->process.file_desciptors, new_process->process.last_fd);
    }
    
    uint64_t rsp = (uint64_t) memory_manager_alloc(4*1024);
    if(rsp == 0) {
        return -1;
    }
    new_process->process.stack_base = rsp;

    uint64_t new_rsp = load_process(rip, rsp + 4 * 1024, argc, (uint64_t) argv);
    new_process->process.rsp = new_rsp;
    
    if (active == NULL) {
        new_process->next = NULL;
        active = new_process;
    } else {
        if (expired == NULL) {
            new_process->next = NULL;
            expired = new_process;
        } else {
            Node * current = expired;
            Node * previous = NULL;
            while (current->next != NULL && new_process->process.priority >= current->process.priority) {
                previous = current;
                current = current->next;
            }
            if (current->next == NULL && new_process->process.priority >= current->process.priority) {
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

void next_to_run() {
    Node * current = active;
    Node * previous = NULL;
    while (current != NULL && current->process.status == BLOCKED) {
        previous = current;
        current = current->next;
    }
    if (current != NULL) {
        if (previous != NULL) {
            previous->next = current->next;
            current->next = active;
            active = current;
        }
    } else {
        Node * aux = active;
        active = expired;
        expired = aux;

        current = active;
        previous = NULL;
        while (current != NULL && current->process.status == BLOCKED) {
            previous = current;
            current = current->next;
        }
        if (previous != NULL) {
            previous->next = current->next;
            current->next = active;
            active = current;
        }
    }
}

int prepare_process_for_work(pid_t pid) {
    Node * current = active;
    Node * previous = NULL;
    while (current != NULL && current->process.pid != pid) {
        previous = current;
        current = current->next;
    }
    if (current != NULL) {
        if (previous != NULL) {
            previous->next = current->next;
            current->next = active;
            active = current;
        }
    } else {
        current = expired;
        previous = NULL;
        while (current != NULL && current->process.pid != pid) {
            previous = current;
            current = current->next;
        }
        if (current == NULL) {
            return -1;
        }
        if (previous == NULL) {
            expired = current->next;   
        } else {
            previous->next = current->next;
        }
        current->next = active;
        active = current;
    }
    return 0;
}

uint64_t context_switch(uint64_t rsp) {
    if (!something_running) {
        something_running = 1;
        if (process_ready_count > 0) {
            next_to_run();
        } else {
            prepare_process_for_work(dummy_process_pid);
        }
        return active->process.rsp;
    }

    Node * current_process = active;
    current_process->process.rsp = rsp;

    if (process_ready_count == 0) {
        prepare_process_for_work(dummy_process_pid);
        return active->process.rsp;
    }

    if (current_process->process.status != BLOCKED && current_process->process.quantums_left > 0) {
        current_process->process.quantums_left--;
        return rsp;
    }

    if (current_process->process.quantums_left == 0) {
        if (current_process->process.new_priority != -1) {
            current_process->process.priority = current_process->process.new_priority;
            current_process->process.new_priority = -1;
        }
        current_process->process.quantums_left = priorities[current_process->process.priority];
        
        Node * current_expired = expired;
        Node * previous_expired = NULL;
        while (current_expired != NULL && current_process->process.priority >= current_expired->process.priority) {
            previous_expired = current_expired;
            current_expired = current_expired->next;
        }
        active = active->next;
        if (previous_expired == NULL) {
            current_process->next = expired;
            expired = current_process;
        } else {
            previous_expired->next = current_process;
            current_process->next = current_expired;
        }
    }
    next_to_run();
    return active->process.rsp;
}

int terminate_process(int return_value, char autokill) {
    Node * current_process = active;

    pid_t blocked_pid;
    while ((blocked_pid = dequeue_pid(current_process->process.blocked_queue)) != -1) {
        unblock_process(blocked_pid);
    }

    active = current_process->next;
    if (current_process->process.status != BLOCKED) {
        process_ready_count--;
    }
    for (int i = 0; i < current_process->process.argc; i++) {
        memory_manager_free(current_process->process.argv[i]);
    }
    memory_manager_free(current_process->process.argv);
    free_queue(current_process->process.blocked_queue);
    memory_manager_free((void *) current_process->process.stack_base);
    memory_manager_free(current_process);
    if (autokill) {
        something_running = 0;
        _int20h();
    }
    return return_value;
}

int change_priority(pid_t pid, int priority_value) {
    if (priority_value < 0 && priority_value > 8) {
        return -1;
    }
    PCB * process = get_process(pid);
    if (process == NULL) {
        return -1;
    }
    process->new_priority = priority_value;
    return 0;
}

PCBInfo * process_info() {
    PCBInfo * info = NULL;

    Queue current = active;
    while (current != NULL) {
        PCBInfo * new_info = memory_manager_alloc(sizeof(PCBInfo));
        new_info->name = strcpy(current->process.argv[0]);
        new_info->pid = current->process.pid;
        new_info->priority = current->process.priority;
        new_info->status = current->process.status;
        new_info->rsp = current->process.rsp;
        new_info->rbp = current->process.rsp;
        new_info->next = info;
        info = new_info;
        current = current->next;
    }
    current = expired;
    while (current != NULL) {
        PCBInfo * new_info = memory_manager_alloc(sizeof(PCBInfo));
        new_info->name = strcpy(current->process.argv[0]);
        new_info->pid = current->process.pid;
        new_info->priority = current->process.priority;
        new_info->status = current->process.status;
        new_info->rsp = current->process.rsp;
        new_info->rbp = current->process.rsp;
        new_info->next = info;
        info = new_info;
        current = current->next;
    }
    return info;
}

int yield_process() {
    active->process.quantums_left = 0;
    _int20h();
    return 0;
}