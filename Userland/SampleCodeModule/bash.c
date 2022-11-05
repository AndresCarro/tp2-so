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

void pipe_manager(char ** parts, int part_count, int pipe_position);
void read_handler(int argc, char * argv[]);
void write_handler(int argc, char * argv[]);

command bck_fun = NULL;
int bck_argc = -1;
char ** bck_argv = NULL;

command pipe_fun[2] = {};
int pipe_argc[2] = {-1,-1};
char ** pipe_argv[2] = {};

extern void halt();

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
        int i;
        char found = 0;
        for (i = 0; !found && i < part_count;) {
            if (strcmp(parts[i], "|") == 0) {
                found = 1;
            } else {
                i++;
            }
        }
        pipe_manager(parts, part_count, i);
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

void pipe_manager(char ** parts, int part_count, int pipe_position) {
    command fun_1 = command_parser(parts[0]);
    if (fun_1 == NULL || fun_1 == BLOCK_BUILTIN || fun_1 == UNBLOCK_BUILTIN || fun_1 == NICE_BUILTIN || fun_1 == KILL_BUILTIN) {
        unknown_command(parts[0]);
        return;
    }
    command fun_2 = command_parser(parts[pipe_position + 1]);
    if (fun_2 == NULL || fun_2 == BLOCK_BUILTIN || fun_2 == UNBLOCK_BUILTIN || fun_2 == NICE_BUILTIN || fun_2 == KILL_BUILTIN) {
        unknown_command(parts[pipe_position + 1]);
        return;
    }
    pipe_fun[0] = fun_1;
    pipe_fun[1] = fun_2;
    pipe_argc[0] = pipe_position;
    pipe_argc[1] = part_count - (pipe_position + 1);
    pipe_argv[0] = parts;
    pipe_argv[1] = &parts[pipe_position + 1];

    int fds[2];
    if (pipe(fds) != 0){
        fprintf(STDOUT, "ERROR: Pipe creation failed\n");
        return;
    }
   
    char fd_r[2];
    char fd_w[2];
    itoa(fds[0], fd_r);
    itoa(fds[1], fd_w);

    char * name_r = "Read Handler";
    char * name_w = "Write Handler";
    char * argv_r[] = {name_r, fd_r, fd_w};
    char * argv_w[] = {name_w, fd_r, fd_w};

    pid_t pid_r = exec((uint64_t) read_handler, 3 ,argv_r);
    pid_t pid_w = exec((uint64_t) write_handler, 3, argv_w);    
    
    close(fds[0]);
    close(fds[1]);
    waitpid(pid_r);
    waitpid(pid_w);
    
    pipe_fun[0] = NULL;
    pipe_fun[1] = NULL;
    pipe_argc[0] = -1;
    pipe_argc[1] = -1;
    pipe_argv[0] = NULL;
    pipe_argv[1] = NULL;
}

void read_handler(int argc, char * argv[]) {
    close(atoi(argv[2]));
    dup2(atoi(argv[1]), STDIN);
    pid_t pid = exec((uint64_t) pipe_fun[1], pipe_argc[1], pipe_argv[1]);
    close(STDIN);
    waitpid(pid);
}

void write_handler(int argc, char * argv[]) {
    close(atoi(argv[1]));
    dup2(atoi(argv[2]), STDOUT);
    pid_t pid = exec((uint64_t) pipe_fun[0], pipe_argc[0], pipe_argv[0]);
    close(STDOUT);
    waitpid(pid);
}

void background_manager(char * name) {
    command fun = command_parser(name + 1);
    if (fun == NULL || fun == BLOCK_BUILTIN || fun == UNBLOCK_BUILTIN || fun == NICE_BUILTIN || fun == KILL_BUILTIN) {
        fprintf(STDOUT, "ERROR. Command < %s > not found or not supported for backround.", name + 1);
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