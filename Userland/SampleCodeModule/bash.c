#include <bash.h>
#include <processes.h>

#define MAX_DIGIT_PID 3
#define COMMAND_COUNT 5

#define BLOCK_BUILTIN 1
#define UNBLOCK_BUILTIN 2
#define NICE_BUILTIN 3
#define KILL_BUILTIN 4

static char buffer[MAX_BUFFER];

static command fun1 = NULL;
static command fun2 = NULL;

int read_input();

command command_parser(char * name);
void unknown_command(char * name);

void block_handler(char * pid);
void unblock_handler(char * pid);
void kill_handler(char * pid);
void nice_handler(char * pid, char * new_priority);

void background_manager(char * name);
void background_handler(int argc, char * argv[]);

command bck_fun = NULL;
int bck_argc = -1;
char ** bck_argv = NULL;

extern void halt();
void pipe_panager();

void bash() {
    int i=0;
    while (i != -1){
        puts("Agodn't.IO:$ ");
        i = read_input();
    }
}

int read_input(){
    int size_read = gets(buffer);

    if (char_belongs(buffer, '|') && char_belongs(buffer, '&')) {
        fprintf(STDOUT, "\nERROR: Piping and background processes are incompatible.\n");
        return 0;
    }

    if (strcmp(buffer, "exit") == 0) {
        fprintf(STDOUT, "\nGoodbye :D\n");
        return -1;
    }
    
    int part_count;
    char ** parts = strtok(buffer, ' ', &part_count);

    put_char('\n');
    if (char_belongs(buffer, '|')) {
        pipe_manager();
    } else if (parts[0][0] == '&') {
        bck_argc = part_count;
        bck_argv = parts;
        background_manager(parts[0]);
    } else {
        command fun = command_parser(parts[0]);
        if (fun == NULL) {
            unknown_command(parts[0]);
        } else if (fun == BLOCK_BUILTIN) {
            if (part_count != 2) {
                fprintf(STDOUT, "ERROR: Must provide only one argument.");
            } else {
                kill_handler(parts[1]);
            }
        } else if (fun == UNBLOCK_BUILTIN) {
            if (part_count != 2) {
                fprintf(STDOUT, "ERROR: Must provide only one argument.");
            } else {
                kill_handler(parts[1]);
            }
        } else if (fun == NICE_BUILTIN) {
            if (part_count != 3) {
                fprintf(STDOUT, "ERROR: Must provide only two arguments.");
            } else {
                nice_handler(parts[1], parts[2]);
            }
        } else if (fun == KILL_BUILTIN) {
            if (part_count != 2) {
                fprintf(STDOUT, "ERROR: Must provide only one argument.");
            } else {
                kill_handler(parts[1]);
            }
        } else {
            pid_t pid = exec((uint64_t) fun, part_count, parts);
            waitpid(pid);
        }
    }
    return size_read;
}

void unknown_command(char * name) {
    fprintf(STDOUT, "ERROR: Unknown command < %s >.", name);
    put_char('\n');
}

void test_nice2() {
    sem_t sem = sem_open("prueba", 0);
    for (int i = 0; i < 100; i++) {
		put_char('B');
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
        put_char('C');
        halt();
    }
}

void test_pipe_2() {
    while (1) {
        puts("\nPipe 2 dice < ");
        char c = get_char();
        put_char(c);
        puts(">");
    }
}

void dup_handler(int argc, char * argv[]) {
    int k = atoi(argv[1]);
    close(atoi(argv[2]));
    dup2(k, STDIN);
    char * name = "Test Pipe 2";
    char * argvs[] = {name};
    exec(test_pipe_2, 1, argvs);
    close(STDIN);
}

void test_pipe() {
    int fds[2];
    pipe(fds);

    char str[2];
    char str2[2];
    itoa(fds[0],str);
    itoa(fds[1],str2);
    char * name = "Dup Handler";
    char * argv[] = {name, str, str2};

    exec((uint64_t) dup_handler, 3, argv);
    close(fds[0]);
    dup2(fds[1], STDOUT);
    while (1) {
        put_char('B');
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

void test_block2() {
    while(1) {
        put_char('B');
        halt();
    }
}

void test_block() {
    char * name = "Test Block 2";
    char * argv[] = {name};
    pid_t pid = exec((uint64_t) test_block2, 1, argv);
    int i = 0;
    while (i < 100) {
        put_char('A');
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
    pid_t prime_pid = exec(print_prime, 1, argv);
    sys_exit(0);
}

command command_parser(char * name){
    if (strcmp(name, "help") == 0) {
        return (command) help;
    } else if (strcmp(name, "mem") == 0) {
        return (command) mem;
    } else if (strcmp(name, "ps") == 0) {
        return (command) ps;
    } else if (strcmp(name, "loop") == 0) {
        return (command) loop;
    } else if (strcmp(name, "sem") == 0) {
        return (command) sem;
    } else if (strcmp(name, "pipe") == 0) {
        return (command) pipe;
    } else if (strcmp(name, "time") == 0) {
        return (command) time;
    } else if (strcmp(name, "primes") == 0) {
        return (command) primes;
    } else if (strcmp(name, "fibonacci") == 0) {
        return (command) fibonacci;
    } else if (strcmp(name, "test_mm") == 0) {
        return (command) test_mm;
    } else if (strcmp(name, "test_processes") == 0) {
        return (command) test_processes;
    } else if (strcmp(name, "test_priority") == 0) {
        return (command) test_priority;
    } else if (strcmp(name, "test_synchro") == 0) {
        return (command) test_synchro;
    } else if (strcmp(name, "wc") == 0) {
        return (command) wc;
    } else if (strcmp(name, "cat") == 0) {
        return (command) cat;
    } else if (strcmp(name, "filter") == 0) {
        return (command) filter;
    }  else if (strcmp(name, "phylo") == 0) {
        return (command) phylo;
    } else if (strcmp(name, "block") == 0) {
        return BLOCK_BUILTIN;
    } else if (strcmp(name, "unblock") == 0) {
        return UNBLOCK_BUILTIN;
    } else if (strcmp(name, "nice") == 0) {
        return NICE_BUILTIN;
    } else if (contains_string(name, "kill") == 0) {
        return KILL_BUILTIN;
    }
    return NULL;
}

void pipe_manager(){
    char cmd1[MAX_SIZE_CMD],cmd2[MAX_SIZE_CMD];
    unsigned int i=0;
    while(name[i] != '|' && i < MAX_SIZE_CMD){
        cmd1[i] = name[i];
        i++;
    }
    if(i == MAX_SIZE_CMD){
        unknown_command(cmd1);
        return;
    }
    cmd1[i] = '\0';
    i++;//como estoy parado en la '|' paso al siguiente
    unsigned int j=0;
    while(name[i] != '\0' && j < MAX_SIZE_CMD){
        cmd2[j++] = name[i++];
    }
    if(j == MAX_SIZE_CMD){
        unknown_command(cmd2);
        return;
    }
    cmd2[j] = '\0';
    fun1 = command_line(cmd1);
    fun2 = command_line(cmd2);
    if(fun1 == NULL || fun2 == NULL){
        return;
    }
    int fds1[2];
    if(pipe(fds1) != 0){
        puts("Error in pipe creation\n");
        return;
    }
    char * name1 = "write_handler";
    char fd_w[2];
    char fd_r[2];
    itoa(fds1[1],fd_w);
    itoa(fds1[0],fd_r);
    char * argv[] = {name1, fd_r,fd_w};
    //pipefd[0] refers to the read end  of  the  pipe. pipefd[1] refers to the write end of the pipe
    pid_t pid1 = exec((uint64_t) write_handler, 3 ,argv);
    
    char * name2 = "read_handler";
    char * argv2[] = {name2, fd_r,fd_w};
    pid_t pid2 = exec((uint64_t) read_handler, 3 ,argv2);
    close(fds1[0]);
    close(fds1[1]);
    waitpid(pid1);
    waitpid(pid2);
    
    fun1 = NULL;
    fun2 = NULL;
}

void write_handler(int argc,char* argv[]){
    if(argc <= 1){
        return;
    }
    close(atoi(argv[1]));
    dup2(atoi(argv[2]),STDOUT);
    char *name = "fun1";
    char *args[] = {name};
    pid_t pid = exec((uint64_t) fun1,1,args);
    close(STDOUT);
    waitpid(pid);
}

void read_handler(int argc,char* argv[]){
    if(argc <= 1){
        return;
    }
    close(atoi(argv[2]));
    dup2(atoi(argv[1]),STDIN);
    char *name = "fun2";
    char *args[] = {name};
    pid_t pid = exec((uint64_t) fun2,1,args);
    close(STDIN);
    waitpid(pid);
}

void background_manager(char * name) {
    command fun = command_parser(name + 1);
    if (fun == NULL || fun == BLOCK_BUILTIN || fun == UNBLOCK_BUILTIN || fun == NICE_BUILTIN || fun == KILL_BUILTIN) {
        fprintf(STDOUT, "ERROR. Command not found or not supported for backround.");
        return;
    }
    bck_fun = fun;

    char * name = "Background Handler";
    char * argv_aux[] = {name};
    pid_t pid = exec((uint64_t) background_handler, 1 , argv_aux);
    waitpid(pid);
    bck_fun = NULL;
    bck_argc = -1;
    bck_argv = NULL;
}

void background_handler(int argc, char * argv[]) {
    close(STDOUT);
    pid_t pid = exec((uint64_t) bck_fun, bck_argc, bck_argv);
}

void block_handler(char * pid) {
    int i;
    for (i = 0; pid[i] != '\0'; i++) {
        if (!is_num(pid[i])) {
            fprintf(STDOUT, "ERROR: Must specify a PID to block.");
            return;
        }
    }
    if (i > MAX_DIGIT_PID) {
        fprintf(STDOUT, "ERROR: PID provided is too big.");
        return;
    }
    if (block(atoi(pid)) == -1) {
        fprintf(STDOUT, "ERROR: Block failed.");
    }
}

void unblock_handler(char * pid) {
    int i;
    for (i = 0; pid[i] != '\0'; i++) {
        if (!is_num(pid[i])) {
            fprintf(STDOUT, "ERROR: Must specify a PID to unblock.");
            return;
        }
    }
    if (i > MAX_DIGIT_PID) {
        fprintf(STDOUT, "ERROR: PID provided is too big.");
        return;
    }
    if (unblock(atoi(pid)) == -1) {
        fprintf(STDOUT, "ERROR: Unblock failed.");
    }
}

void kill_handler(char * pid) {
    int i;
    for (i = 0; pid[i] != '\0'; i++) {
        if (!is_num(pid[i])) {
            fprintf(STDOUT, "ERROR: Must specify a PID to kill.");
            return;
        }
    }
    if (i > MAX_DIGIT_PID) {
        fprintf(STDOUT, "ERROR: PID provided is too big.");
        return;
    }
    if (kill(atoi(pid)) == -1) {
        fprintf(STDOUT, "ERROR: Kill failed.");
    }
}

void nice_handler(char * pid, char * new_priority) {
    int i;
    for (i = 0; pid[i] != '\0'; i++) {
        if (!is_num(pid[i])) {
            fprintf(STDOUT, "ERROR: Must specify a PID to change its priority.");
            return;
        }
    }
    if (i > MAX_DIGIT_PID) {
        fprintf(STDOUT, "ERROR: PID provided is too big.");
        return;
    }
    for (i = 0; new_priority[i] != '\0'; i++) {
        if (!is_num(pid[i])) {
            fprintf(STDOUT, "ERROR: Must specify a value for the new priority.");
            return;
        }
    }
    if (nice(atoi(pid), atoi(new_priority)) == -1) {
        fprintf(STDOUT, "ERROR: Nice failed.");
    }
}