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
            
    }
    return 0;
}

static uint64_t sys_read(unsigned int fd,char* output, uint64_t count){
    switch (fd)
    {
    case STDIN:
        return readBuffer(output, count);
        break;
    
    default:
        return 0;
    }
}

static void sys_write(unsigned fd,const char* buffer, uint64_t count){
    uint64_t i = 0;
    while (i < count)
    {
        switch(fd){
            case STDOUT:
                ncPrintChar(buffer[i]);
                break;
            case STDERR:
                ncPrintCharFormat(buffer[i], ERROR_FORMAT);
                break;
            default:
                return;
        }
        i++;
    }
}

static pid_t sys_exec(uint64_t program, unsigned int argc, char * argv[]){
    return create_process(program, argc, argv);
}

static void sys_exit(int return_value){
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