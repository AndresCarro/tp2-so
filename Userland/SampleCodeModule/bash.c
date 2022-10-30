#include <bash.h>

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
            pid_t pid = exec((uint64_t) fun, 0, NULL);
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
    exec((uint64_t) test_nice2, 0, NULL);
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
    exec(test_pipe_2, 0, NULL);
}

void test_pipe() {
    int fds[2];
    pipe(fds);

    char str[2];
    itoa(fds[0],str);
    char * argv[] = {str};

    exec((uint64_t) dup_handler, 1, argv);
    
    dup2(fds[1], STDOUT);
    while (1) {
        prueba();
        putChar('B');
    }
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