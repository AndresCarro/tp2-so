#include <bash.h>
#include <processes.h>

#define MAX_SIZE_CMD 32
#define MAX_DIGIT_PID 3

static char buffer[32];
static pm fun1 = NULL;
static pm fun2 = NULL;

int read_input();
void unknown_command();
void pipe_panager();
void background_manager();
pm command_line(char* buffer);

extern void halt();

void bash() {
    int i=0;
    while(i != -1){
        puts("Agodn't.IO:$ ");
        i = read_input();
        // put_char('\n');
    }
}

/*
PROCESO PADRE -> EXEC A PROCESO INTERMEDIO
PROCESO INTERMEDIO -> DUP -> EXEC A PROCESO HIJO -> EXIT()
PROCESO HIJO -> CORRE
*/

int read_input(){
    int size_read = gets(buffer);
    if(strcmp(buffer,"exit") == 0) {
        puts("\nGoodbye :D\n");
        return -1;
    } else if(char_belongs(buffer,'|')) {
        put_char('\n');
        pipe_manager();
    } else if(char_belongs(buffer,'&')) {
        put_char('\n');
        background_manager();
    } else {
        put_char('\n');
        pm fun = command_line(buffer);
        if(fun != NULL){
            char * name = "Funcion";
            char * argv[] = {name}; // TODO HAY QUE ARREGLAR ESTO
            pid_t pid = exec((uint64_t) fun, 1, argv);
            waitpid(pid);
        }
    }
    //etc, para los distintos comandos a implementar
    return size_read;
}

void unknown_command(){
    puts("\nUnknown command: ");
    puts(buffer);
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

pm command_line(char* buffer){
    // put_char('\n');
    if(strcmp(buffer,"time") == 0){
        return (pm)get_time;
    } else if (strcmp(buffer,"prime") == 0){
        return (pm)print_prime;
    } else if (strcmp(buffer,"fibonacci") == 0){
        return (pm)fibonacci_numbs;
    } else if (strcmp(buffer,"dividebyzero") == 0){
        return (pm)excep_div_zero;
    } else if (strcmp(buffer,"help") == 0){
        return (pm)help;
    } else if (strcmp(buffer,"invalidopcode") == 0){
        return (pm)excep_invalid_opcode;
    } else if (strcmp(buffer, "test_nice") == 0) {
        return (pm) test_nice;
    } else if (strcmp(buffer, "test_pipes") == 0) {
        return (pm) test_pipe;
    } else if (strcmp(buffer, "test_close") == 0) {
        return (pm) test_close;
    } else if (strcmp(buffer, "test_pipe_info") == 0) {
        return (pm) test_pipe_info;
    } else if (strcmp(buffer, "test_sem_info") == 0) {
        return (pm) test_sem_info;
    } else if (strcmp(buffer, "ps") == 0) {
        return (pm) print_process_info;
    } else if (strcmp(buffer, "mem") == 0) {
        return (pm) print_mem_info;
    } else if (strcmp(buffer, "test mm") == 0) {
        char * name = "Memory Test";
        char * max_memory = "17000000";
        char * argv[] = {name, max_memory};
        pid_t pid = exec((uint64_t) test_mm, 2, argv);
        waitpid(pid);
    } else if (strcmp(buffer, "test processes") == 0) {
        char * name = "Processes Test";
        char * max_proc = "5";
        char * argv[] = {name, max_proc};
        pid_t pid = exec((uint64_t) test_processes, 2, argv);
        waitpid(pid);
    } else if (strcmp(buffer, "test_p") == 0) {
        char * name = "Our Processes Test";
        char * argv[] = {name};
        pid_t pid = exec((uint64_t) test_block, 1, argv);
        waitpid(pid);
    } else if (strcmp(buffer, "test_prio") == 0) {
        char * name = "Priorities aGODio Test";
        char * argv[] = {name};
        pid_t pid = exec((uint64_t) test_prio, 1, argv);
        waitpid(pid);
    } else if (strcmp(buffer, "test_sync_with_sem") == 0) {
        char * name = "Sync aGODio Test with sem";
        char * argv[] = {name, "10", "1"};
        pid_t pid = exec((uint64_t) test_sync, 3, argv);
        waitpid(pid);
    } else if (strcmp(buffer, "test_sync_without_sem") == 0) {
        char * name = "Sync aGODio Test without sem";
        char * argv[] = {name, "60", "0"};
        pid_t pid = exec((uint64_t) test_sync, 3, argv);
        exec((uint64_t) medium, 3, argv);
        waitpid(pid);
    } else if (strcmp(buffer, "pb") == 0) {
        char * name = "Primes SCREEN";
        char * name2 = "Primes BCK";
        char * argv[] = {name};
        char * argv2[] = {name2};
        pid_t pid = exec((uint64_t) print_prime, 1, argv);
        exec((uint64_t) medium, 1, argv2);
        waitpid(pid);
    } else if (strcmp(buffer, "wc") == 0) {
        return (pm) wc;
    } else if (strcmp(buffer, "loop") == 0) {
        return (pm) loop;
    } else if (contains_string(buffer, "kill") == 0) {
        kill_handler();
        return NULL;
    } else if (contains_string(buffer, "nice") == 0) {
        // nice_handler();
        return NULL;
    } else if (strcmp(buffer, "sem") == 0) {
        return (pm) print_sem_info;
    } else if (strcmp(buffer, "pipe") == 0) {
        return (pm) print_pipe_info;
    } else {
        unknown_command(buffer);
    }
    return NULL;
}

void pipe_manager(){
    char cmd1[MAX_SIZE_CMD],cmd2[MAX_SIZE_CMD];
    unsigned int i=0;
    while(buffer[i] != '|' && i < MAX_SIZE_CMD){
        cmd1[i] = buffer[i];
        i++;
    }
    if(i == MAX_SIZE_CMD){
        unknown_command(cmd1);
        return;
    }
    cmd1[i] = '\0';
    i++;//como estoy parado en la '|' paso al siguiente
    unsigned int j=0;
    while(buffer[i] != '\0' && j < MAX_SIZE_CMD){
        cmd2[j++] = buffer[i++];
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

void background_manager(){
    char cmd[MAX_SIZE_CMD];
    unsigned int i=0;
    while(buffer[i] != '&' && i < MAX_SIZE_CMD){
        cmd[i] = buffer[i];
        i++;
    }
    if(i == MAX_SIZE_CMD){
        unknown_command();
    }
    fun1 = command_line(cmd);
    if(fun1 == NULL){
        return;
    }
    char * name = "background_handler";
    char * argv[] = {name};
    pid_t pid = exec((uint64_t) background_handler, 1 ,argv);
    waitpid(pid);
    fun1 = NULL;
}

void background_handler(int argc,char* argv[]){
    close(STDOUT);
    char *name = "background_fun";
    char *argv2[] = {name};
    pid_t pid = exec((uint64_t) fun1,1,argv2);
}

void kill_handler(){
    char pid[MAX_DIGIT_PID+1];
    char check_cmd[4];
    int i=0;
    while( buffer[i] != '\0' && i < MAX_SIZE_CMD && !is_num(buffer[i])){
        check_cmd[i] = buffer[i];
        i++;
    }
    if( i == MAX_SIZE_CMD || strcmp(check_cmd,"kill") != 0 ){
        unknown_command();
        return;
    }
    int j=0;
    while ( buffer[i] != '\0' ){
        if( is_num(buffer[i]) ){
            pid[j++] = buffer[i++];
        }else{
            unknown_command();
            return;
        }
    }
    pid[j] = '\0'; 
    if( j > MAX_DIGIT_PID ){
        puts("Invalid process id\n");
        return;
    }
    if(kill(atoi(pid)) == -1){
        puts("Error in the killing process\n");
    }
}

void nice_handler(){
    int nice_value;
    char check_cmd[4];
    int i=0;
    while( buffer[i] != '\0' && i < MAX_SIZE_CMD && !is_num(buffer[i])){
        check_cmd[i] = buffer[i];
        i++;
    }
    if( i == MAX_SIZE_CMD || strcmp(check_cmd,"nice") != 0 ){
        return;
    }
    int i;

    
    if(is_num(buffer[i])){
        i = buffer[i] - '0';
        if(nice() == -1){

        }
    }
}