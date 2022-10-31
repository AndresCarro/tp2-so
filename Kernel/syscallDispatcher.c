#include <syscallDispatcher.h>
#include <defs.h>
#include <scheduler2.h>
#include <memory_manager.h>
#include <semaphore.h>

static uint64_t sys_read(unsigned int fd, char* output, uint64_t count);
static void sys_write(unsigned fd, const char* buffer, uint64_t count);
static pid_t sys_exec(uint64_t program, unsigned int argc, char * argv[]);
static void sys_exit(int retValue);
static pid_t sys_waitpid(pid_t pid);
static int sys_nice(int new_priority);

static void sys_time(time_t * s);
static void sys_copymem(uint64_t address, uint8_t * buffer, uint64_t length);
static void * sys_malloc(uint64_t size);
static void sys_free(uint64_t ptr);
static void sys_get_mem_state(uint64_t memory_state);
static sem_t sys_sem_open(char * name, int initial_value);
static void sys_sem_close(sem_t sem);
static int sys_sem_wait(sem_t sem);
static int sys_sem_post(sem_t sem);
static int sys_pipe(int fds[]);
static void sys_dup2(int old, int new);
static void sys_close(int fd);

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax, uint64_t * registers){
    switch(rax){
        case 0:
            return sys_read((unsigned int)rdi, (char*)rsi,rdx);
            break;
        case 1:
            sys_write((unsigned int)rdi, (char*)rsi,rdx);
            break;
        case 2:
            return (uint64_t) getRegisters((uint64_t *)rdi);
            break;
        case 3:
            return sys_exec(rdi, (unsigned int) rsi, (char **) rdx);
            break;
        case 4:
            sys_exit((int) rdi);
            break;
        case 5:
            sys_time((time_t*)rdi);
            break;
        case 6:
            sys_copymem(rdi,(uint8_t *) rsi, rdx);
            break;
        case 7:
            return (uint64_t) sys_malloc(rdi);
            break;
        case 8:
            sys_free(rdi);
            break;
        case 9:
            sys_get_mem_state(rdi);
            break;
        case 10:
            return sys_waitpid((pid_t) rdi);
            break;
        case 11:
            return sys_nice((int) rdi);
            break;
        case 12:
            return sys_sem_open((char *) rdi, (int) rsi);
            break;
        case 13:
            sys_sem_close((sem_t) rdi);
            break;
        case 14:
            return sys_sem_wait((sem_t) rdi);
            break;
        case 15:
            return sys_sem_post((sem_t) rdi);
            break;
        case 16:
            return sys_pipe((int *) rdi);
            break;
        case 17:
            sys_dup2((int) rdi, (int) rsi);
            break;
        case 18:
            sys_close((int) rdi);
            break;
    }
    return 0;
}

static uint64_t sys_read(unsigned int fd, char * output, uint64_t count){
    PCB * pcb = get_process(get_current_pid());
    fd_t * table = pcb->file_desciptors;
    unsigned int last_fd = pcb->last_fd;

    if (fd >= last_fd || table[fd].mode != READ) {
        return -1;
    }
    return pipe_read(table[fd].pipe, output, count);
}

static void sys_write(unsigned fd, const char * buffer, uint64_t count){
    PCB * pcb = get_process(get_current_pid());
    fd_t * table = pcb->file_desciptors;
    unsigned int last_fd = pcb->last_fd;

    if (fd >= last_fd || table[fd].mode != WRITE) {
        return;
    }

    if (fd == STDOUT && table[fd].pipe == NULL) {
        for (int i = 0; i < count; i++) {
            ncPrintChar(buffer[i]);
        }
        return;
    }
    return pipe_write(table[fd].pipe, buffer, count);
}

static pid_t sys_exec(uint64_t program, unsigned int argc, char * argv[]){
    return create_process(program, argc, argv);
}

static void sys_exit(int return_value){
    PCB * pcb = get_process(get_current_pid());
    unsigned int last_fd = pcb->last_fd;

    for (int i = 0; i < last_fd; i++) {
        sys_close(i);
    }

    terminate_process(return_value);
}

static pid_t sys_waitpid(pid_t pid) {
    PCB * process_pcb = get_process(pid);
    if (process_pcb == NULL) {
        return -1;
    }

    pid_t current_pid = get_current_pid();
    enqueue_pid(process_pcb->blocked_queue, current_pid);
    block_process(current_pid);

    return pid;
}

static int sys_nice(int new_priority) {
    return change_priority(new_priority);
}

static void sys_time(time_t * s){
    s->day = localDay();
    s->month = localMonth();
    s->year = localYear();
    s->hours = localHours();
    s->minutes = getMinutes();
    s->seconds = getSeconds();
}

static void sys_copymem(uint64_t address, uint8_t * buffer, uint64_t length){
    memcpy((void*)buffer, (void*)address, length);
}

static void * sys_malloc(uint64_t size) {
    return memory_manager_alloc(size);
}

static void sys_free(uint64_t ptr) {
    memory_manager_free((void *) ptr);
}

static void sys_get_mem_state(uint64_t memory_state) {
    memory_manager_get_state((Memory_State *)memory_state);
}

static sem_t sys_sem_open(char * name, int initial_value) {
    return sem_open(name, initial_value);
}

static void sys_sem_close(sem_t sem) {
    sem_close(sem);
}

static int sys_sem_wait(sem_t sem) {
    return sem_wait(sem);
}

static int sys_sem_post(sem_t sem) {
    return sem_post(sem);
}

static int sys_pipe(int fds[]) {
    PCB * pcb = get_process(get_current_pid());
    fd_t * table = pcb->file_desciptors;
    unsigned int last_fd = pcb->last_fd;

    int available = 0;
    int read_fd = 0;
    while (available != 1 && read_fd < MAX_FDS && last_fd < MAX_FDS) {
        if (read_fd == last_fd) {
            available++;
            last_fd++;
        } else if (table[read_fd].mode == CLOSED) {
            available++;
        } else {
            read_fd++;
        }
    }
    int write_fd = read_fd + 1;
    while (available != 2 && write_fd < MAX_FDS && last_fd < MAX_FDS) {
        if (write_fd == last_fd) {
            available++;
            last_fd++;
        } else if (table[write_fd].mode == CLOSED) {
            available++;
        } else {
            write_fd++;
        }
    }

    if (available != 2) {
        return -1;
    }

    Pipe * new_pipe = pipe_open();
    table[read_fd].mode = READ;
    table[read_fd].pipe = new_pipe;
    fds[0] = read_fd;
    table[write_fd].mode = WRITE;
    table[write_fd].pipe = new_pipe;
    fds[1] = write_fd;

    pcb->last_fd = last_fd;

    return 0;
}

static void sys_dup2(int old, int new) {
    PCB * pcb = get_process(get_current_pid());
    fd_t * table = pcb->file_desciptors;
    unsigned int last_fd = pcb->last_fd;

    if (old >= last_fd || new >= last_fd || table[old].mode == CLOSED || table[new].mode == CLOSED) {
        return;
    }
    table[new].mode = table[old].mode;
    table[new].pipe = table[old].pipe;
}

static void sys_close(int fd) {
    PCB * pcb = get_process(get_current_pid());
    fd_t * table = pcb->file_desciptors;
    unsigned int last_fd = pcb->last_fd;

    if (fd >= last_fd) {
        return;
    }

    if (table[fd].pipe != NULL) {
        pipe_close(table[fd].pipe, table[fd].mode == WRITE);
    }
    table[fd].mode = CLOSED;

    while (table[last_fd - 1].mode == CLOSED) {
        last_fd--;
    }
    
    pcb->last_fd = last_fd;
}