// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processes.h>
#include <syslib.h>
#include <types.h>

void help(int argc, char * argv[]) {
    char * helpstring = "block <pid>                   Blocks the given process.\n"
                        "cat                           Prints what it reads from standard input.\n"
                        "clear                         Clears the screen.\n"
                        "fibonacci                     Displays Fibonacci series.\n"
                        "filter                        Prints what it reads from STDIN without vowels.\n"
                        "help                          Provides information about available commands.\n"
                        "kill <pid>                    Kills the given process.\n"
                        "loop                          Infinite loop which greets you once in a while.\n"
                        "mem                           Displays memory state.\n"
                        "nice <pid> <prio>             Change the given process priority.\n"
                        "phylo                         Graphic solution to the Phylosophers' problem.\n"
                        "pipes                         Displays pipes state.\n"
                        "primes                        Prints all primes.\n"
                        "ps                            Displays processes state.\n"
                        "test_mm <max_mem>             Process to test memory allocation.\n"
                        "test_priority                 Process to test nice functionality.\n"
                        "test_processes <max_proc>     Process to test process creation.\n"
                        "test_synchro <iter> <use_sem> Process to test semaphores.\n"
                        "time                          Dispalys current date and time (GMT-3).\n"
                        "turtle                        Prints the OS mascot.\n"
                        "unblock <pid>                 Unblocks the given process.\n"
                        "sem                           Displays semaphores state.\n"
                        "wc                            Prints line count of STDIN.\n"
                        "For more information, read the User Manual.\n";
    fprintf(STDOUT, helpstring);
}