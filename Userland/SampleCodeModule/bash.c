#include <bash.h>
#include <processes.h>

#define MAX_SIZE_CMD 32
static char buffer[32];

void help();
int readInput();
void unknownCommand();
void pipeManager();
pm commandLine(char* buffer);

extern void halt();

void bash() {
    int i=0;
    while(i != -1){
        puts("Horac.IO:$ ");
        i = readInput();
        putChar('\n');
    }
}

/*
PROCESO PADRE -> EXEC A PROCESO INTERMEDIO
PROCESO INTERMEDIO -> DUP -> EXEC A PROCESO HIJO -> EXIT()
PROCESO HIJO -> CORRE
*/

int readInput(){
    int sizeRead = gets(buffer);
    if(strcmp(buffer,"exit") == 0){
        puts("\nGoodbye\n");
        return -1;
    }else if(charBelongs(buffer,'|')){
        putChar('\n');
        // pipeManager();
    }else{
        pm fun = commandLine(buffer);
        if(fun != NULL){
            char * name = "Funcion";
            char * argv[] = {name}; // TODO HAY QUE ARREGLAR ESTO
            pid_t pid = exec((uint64_t) fun, 1, argv);
            waitpid(pid);
        }
    }
    //etc, para los distintos comandos a implementar
    return sizeRead;
}

void unknownCommand(){
    puts("\nUnknown command: ");
    puts(buffer);
    putChar('\n');
}

void test_nice2() {
    sem_t sem = sem_open("prueba", 0);
    for (int i = 0; i < 100; i++) {
		putChar('B');
		if (i == 50) {
			sem_post(sem);
		}
        halt();
    }
}

void test_nice() {
    char * name = "Test Nice 2";
    char * argv[] = {name};
    exec((uint64_t) test_nice2, 1, argv);
    sem_t sem = sem_open("prueba", 0);
	sem_wait(sem);
    while (1) {
        putChar('C');
        halt();
    }
}

void prueba() {

}

void test_pipe_2() {
    while (1) {
        prueba();
        puts("\nPipe 2 dice < ");
        char c = getChar();
        putChar(c);
        puts(">");
    }
}

void dup_handler(int argc, char * argv[]) {
    int k = atoi(argv[0]);
    dup2(k, STDIN);
    char * name = "Test Pipe 2";
    char * argvs[] = {name};
    exec(test_pipe_2, 1, argvs);
}

void test_pipe() {
    int fds[2];
    pipe(fds);

    char str[2];
    itoa(fds[0],str);
    char * name = "Dup Handler";
    char * argv[] = {name, str};

    exec((uint64_t) dup_handler, 2, argv);
    
    dup2(fds[1], STDOUT);
    while (1) {
        prueba();
        putChar('B');
    }
}

void test_close() {
    int fds1[2];
    int fds2[2];
    pipe(fds1);
    pipe(fds2);
    close(fds2[1]);
    int fds3[2];
    pipe(fds3);
    if (fds3[0] == fds2[1]) {
        puts("FUNCIONA");
    }
}

void test_pipe_info() {
    int fds1[2];
    int fds2[2];
    pipe(fds1);
    pipe(fds2);
    sys_write(fds1[1], "HOLA HOLA HOLA", 14);
    close(fds2[0]);

    PipeInfo * info = pipe_info();
    while (info != NULL) {
        fprintf(STDOUT, "Avail: %d; Left: %d, Open Read: %d, Open Write: %d\n", info->available_space, info->left_to_read, info->open_for_read, info->open_for_write);
        info = info->next;
    }
}

void test_sem_info_2() {
    sem_t sem = sem_open("prueba", 0);
    sem_t sem2 = sem_open("prueba2", 3);
    sem_post(sem2);
    
    SemInfo * info = sem_info();
    while (info != NULL) {
        fprintf(STDOUT, "Name: %s, Value: %d, Blocked Processes: %d, Linked Processes: %d\n", info->name, info->value, info->blocked_processes, info->linked_processes);
        info = info->next;
    }
    sem_post(sem);
}

void test_sem_info() {
    char * name = "Test Sem Info 2";
    char * argv[] = {name};
    exec((uint64_t) test_sem_info_2, 1, argv);
    sem_t sem = sem_open("prueba", 0);
	sem_wait(sem);
}

void test_process_info() {
    PCBInfo * info = process_info();
    char * status[] = {"Ready", "Blocked", "Terminated"};
    while (info != NULL) {
        fprintf(STDOUT, "Name: %s, PID: %d, RSP: 0x%x, RBP: 0x%x, Priority: %d, Status: %s\n", info->name, info->pid, info->rsp, info->rbp, info->priority, status[info->status]);
        info = info->next;
    }
}

void test_mem_info() {
    MemInfo * info = mem_info();
    fprintf(STDOUT, "Total: %d, Occupied: %d, Free: %d, Fragments: %d\n", info->memory_total, info->memory_occupied, info->memory_free, info->memory_frags);
}

void test_block2() {
    while(1) {
        putChar('B');
        halt();
    }
}

void test_block() {
    char * name = "Test Block 2";
    char * argv[] = {name};
    pid_t pid = exec((uint64_t) test_block2, 1, argv);
    int i = 0;
    while (i < 100) {
        putChar('A');
        if (i == 10) {
            block(pid);
        } else if (i == 20) {
            unblock(pid);
        } else if (i == 40) {
            kill(pid);
        }
        halt();
        i++;
    }
}

void medium () {
    close(STDOUT);
    char * name = "Primes BCK";
    char * argv[] = {name};
    pid_t prime_pid = exec(printPrime, 1, argv);
    sys_exit(0);
}

void prueba1() {

}

pm commandLine(char* buffer){
    if(strcmp(buffer,"time") == 0){
        putChar('\n');
        return (pm)getTime;
    }else if(strcmp(buffer,"prime") == 0){
        putChar('\n');
        return (pm)printPrime;
    }else if(strcmp(buffer,"fibonacci") == 0){
        putChar('\n');
        return (pm)fibonacciNumbs;
    }else if(strcmp(buffer,"inforeg") == 0){
        putChar('\n');
        return (pm)inforeg;
    }else if(strcmp(buffer,"dividebyzero") == 0){
        putChar('\n');
        return (pm)excepDivZero;
    }else if(strcmp(buffer,"help") == 0){
        putChar('\n');
        return (pm)help;
    }else if(strcmp(buffer,"invalidopcode") == 0){
        putChar('\n');
        return (pm)excepInvalidOpcode;
    }else if(containsString(buffer,"printmem") >= 0){
        putChar('\n');
        savePrintMemParams(buffer);
        return (pm)printmem;
    }else if( (strcmp(buffer,"inforeg")) == 0){
        putChar('\n');
        return (pm)inforeg;
    } else if (strcmp(buffer, "test_nice") == 0) {
        putChar('\n');
        return (pm) test_nice;
    }  else if (strcmp(buffer, "test_pipes") == 0) {
        putChar('\n');
        return (pm) test_pipe;
    } else if (strcmp(buffer, "test_close") == 0) {
        putChar('\n');
        return (pm) test_close;
    } else if (strcmp(buffer, "test_pipe_info") == 0) {
        putChar('\n');
        return (pm) test_pipe_info;
    } else if (strcmp(buffer, "test_sem_info") == 0) {
        putChar('\n');
        return (pm) test_sem_info;
    } else if (strcmp(buffer, "test_process_info") == 0) {
        putChar('\n');
        return (pm) test_process_info;
    } else if (strcmp(buffer, "mem info") == 0) {
        putChar('\n');
        return (pm) test_mem_info;
    } else if (strcmp(buffer, "test mm") == 0) {
        putChar('\n');
        char * name = "Memory Test";
        char * max_memory = "17000000";
        //char * max_memory = "4999999";
        //char * max_memory = "20000000";
        char * argv[] = {name, max_memory};
        pid_t pid = exec((uint64_t) test_mm, 2, argv);
        waitpid(pid);
    } else if (strcmp(buffer, "test processes") == 0) {
        putChar('\n');
        char * name = "Processes Test";
        char * max_proc = "5";
        char * argv[] = {name, max_proc};
        pid_t pid = exec((uint64_t) test_processes, 2, argv);
        waitpid(pid);
    } else if (strcmp(buffer, "test_p") == 0) {
        putChar('\n');
        char * name = "Our Processes Test";
        char * argv[] = {name};
        pid_t pid = exec((uint64_t) test_block, 1, argv);
        waitpid(pid);
    } else if (strcmp(buffer, "test_prio") == 0) {
        putChar('\n');
        char * name = "Priorities aGODio Test";
        char * argv[] = {name};
        pid_t pid = exec((uint64_t) test_prio, 1, argv);
        waitpid(pid);
    } else if (strcmp(buffer, "test_sync_with_sem") == 0) {
        putChar('\n');
        char * name = "Sync aGODio Test with sem";
        char * argv[] = {name, "10", "1"};
        pid_t pid = exec((uint64_t) test_sync, 3, argv);
        waitpid(pid);
    } else if (strcmp(buffer, "test_sync_without_sem") == 0) {
        prueba1();
        putChar('\n');
        char * name = "Sync aGODio Test without sem";
        char * argv[] = {name, "60", "0"};
        pid_t pid = exec((uint64_t) test_sync, 3, argv);
        exec((uint64_t) medium, 3, argv);
        waitpid(pid);
    } else if (strcmp(buffer, "pb") == 0) {
        prueba1();
        putChar('\n');
        char * name = "Primes SCREEN";
        char * name2 = "Primes BCK";
        char * argv[] = {name};
        char * argv2[] = {name2};
        pid_t pid = exec((uint64_t) printPrime, 1, argv);
        exec((uint64_t) medium, 1, argv2);
        waitpid(pid);
    }
    return NULL;
}

// void pipeManager(){
//     char cmd1[MAX_SIZE_CMD],cmd2[MAX_SIZE_CMD];
//     unsigned int i=0;
//     while(buffer[i] != '|' && i < MAX_SIZE_CMD){
//         cmd1[i] = buffer[i];
//         i++;
//     }
//     if(i == MAX_SIZE_CMD){
//         unknownCommand(cmd1);
//         return;
//     }
//     cmd1[i] = '\0';
//     i++;//como estoy parado en la '|' paso al siguiente
//     unsigned int j=0;
//     while(buffer[i] != '\0' && j < MAX_SIZE_CMD){
//         cmd2[j++] = buffer[i++];
//     }
//     if(j == MAX_SIZE_CMD){
//         unknownCommand(cmd2);
//         return;
//     }
//     cmd2[j] = '\0';
//     pm fun1 = commandLine(cmd1);
//     pm fun2 = commandLine(cmd2);
//     if(fun1 == NULL || fun2 == NULL){
//         return;
//     }
//     sys_execve((void(*)())fun1,(void(*)())fun2);
// }

void help(){
    const char* helpstring = 
    "help                 Provides help information for commands.\n"
	"dividebyzero         Command to verify the operation of the exception routine\n"
    "                     \"Divide by zero\"\n"
    "invalidopcode        Command to verify the operation of the exception routine\n"
    "                     \"Invalid Opcode\"\n"
	"inforeg              CTRL+C takes a screenshot of registers, inforeg prints on\n"
    "                     screen the value of all registers screenshoted.\n"
	"printmem             Receives as argument a pointer and performs a memory dump\n"
    "                     of 32 bytes from the address received as an argument.\n"
    "                     Format for address has to start with 0x and be followed up\n"
    "                     with the actual address written in hex.\n"
	"time                 Command to display the system day and time.\n"
    "prime                Dispalys prime numbers starting from 1.\n"
    "fibonacci            Dispalys fibonacci series numbers.\n"
    "COMMAND1|COMMAND2    The \"|\" operand allows the execution of multiple commands\n"
    "                     at the same time. CTRL+L and CTRL+R stops and resumes left\n"
    "                     and right windows. CTRL+E stops both windows and returns\n"
    "                     to console.\n";
    
    puts(helpstring);
}