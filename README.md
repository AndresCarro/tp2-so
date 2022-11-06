# 🐢 turtleOS

turtleOS is a small operating system developed as from Pure64.

It is possible to interact with the OS through a terminal, which allows to execute several commands to verify its functionalities.

For it to be used, it is necessary to have access to a keyboard; the mouse is not needed.

> ‼️ turtleOS assumes a keyboard with ["United States" ANSI distribution](https://upload.wikimedia.org/wikipedia/commons/5/51/KB_United_States-NoAltGr.svg) is used.

# Compiling and Executing

### Prerequisites

The following packages are needed to compile and run the project.

- nasm
- qemu
- gcc
- make
- docker

### Compiling

Only once, download the Docker image:

```bash
docker pull agodio/itba-so:1.0
```

Then, whenever compiling is needed, execute the following command in the root directory of the project:

```bash
docker run -v "${PWD}:/root" --privileged -ti --rm agodio/itba-so:1.0
```

The Docker container will start. Now, execute the following commands to compile the Toolchain.

```bash
cd root
cd Toolchain
make all
cd ..
```

After that, we need to compile the project. There are two memory management algorithms available, and one must be chosen while compiling.

To compile turtleOS with Heap memory manager, execute:

```bash
make all
```

To compile turtleOS with Buddy memory manager, execute:

```bash
make buddy
```

Now exit the Docker container:

```bash
exit
```

### Static Code Analysis

The operating system is prepared y passes without any errors nor warnings the following static-code analyzers.

- PVS-Studio
- CPPCheck

To execute said analyzers, run the following commands in the root directory of the project.

```bash
docker run -v "${PWD}:/root" --privileged -ti --rm agodio/itba-so:1.0
cd root
cd Toolchain
make all
cd ..
make static_analysis
```

PVS-Studio results will be available on the file `report.tasks`. Moreover, CPPCheck results will be available on the file `cppcheck.txt`.

### Execution

In a host with QEMU installed, run:

```bash
./run.sh
```

# Available Commands

### `block <pid>`

Program that inconditionally blocks the program which the provided process ID references to.

> ⚠️ This is a **built-in** program, which means that it is not a process per se, thus it cannot be executed in **background** nor used with **pipes**.

### `cat`

Program that prints to standard output what it receives from standard input.

The program ends when it reads `EOF (-1)`. Said character can be entered on the keyboard with the shortcut `Ctrl + D`.

### `clear`

Program that cleans the screen and positions the cursor on the upper left corner.

> ⚠️ This is a **built-in** program, which means that it is not a process per se, thus it cannot be executed in **background** nor used with **pipes**.

### `fibonacci`

Program that prints to screen the Fibonacci series numbers.

The program ends when the next number of the series is not representable.

### `filter`

Program that prints to standard output what it receives from standard input, except the vowels.

The program ends when it reads `EOF (-1)`. Said character can be entered on the keyboard with the shortcut `Ctrl + D`.

### `help`

Program which displays a list of valid commands on screen.


### `kill <pid>`

Program that ends the execution of the program which the provided process ID references to.

> ⚠️ This is a **built-in** program, which means that it is not a process per se, thus it cannot be executed in **background** nor used with **pipes**.

### `loop`

Program that greets you and "sleeps" for a specified amount of time.

The program does not end, it enters on an infinite loop.

> ℹ️ With educational purpuses, this process executes busy waiting. This is allowed to check the correct functionality of the scheduling algorithm.

### `mem`

Program that prints the OS' memory state.

### `nice <pid> <new_priority>`

Program that changes the priority of the program which the provided process ID references to.

The priority must be an integer greater or equal than 0 and less or equal than 8. The lowest priority is 8, and the greatest priority is 0.

> ⚠️ This is a **built-in** program, which means that it is not a process per se, thus it cannot be executed in **background** nor used with **pipes**.

### `phylo`

Graphic solution to the phylosophers' problem.

Adding and removing phylosophers at runtime is allowed by using `a` y `r`, respectively.

The program ends when `q` is pressed.

### `pipes`

Program that prints the OS' pipes state.

### `primes`

Program that prints to screen the primes starting by 2.

The program ends when the next prime is not representable.

### `ps`

Program that prints the OS' processes state.

### `test_mm <max_memory>`

Program that executes the provided tests to verify the functionality of the memory manager.

The given argument represents how much memory will be allocated.

### `test_priority`

Program that executes the provided tests to verify the functionality of process priority changing.

### `test_processes <max_processes>`

Program that executes the provided tests to verify the functionality of the creation, termination, blocking and unblocking of processes.

### `test_synchro <iterations> <use_sem>`

Program that executes the provided tests to verify the functionality of the semaphores implementation.

The first argument represents the amount of times a variable will be modified to verify the functionality of the semaphores.

The second argument represents a flag which will define if the test is run with semaphores enabled or not.

If the program is executed while using semaphores, the final value must be zero. If not, this final value may or may not be zero.

### `time`

Program that prints on screen the local date and time. Said date and time is displayed in GMT-3 timezone.

### `turtle`

Program that prints to standard output the OS mascot.

### `unblock <pid>`

Program that unblocks the program which the provided process ID references to.

The priority must be an integer greater or equal than 0 and less or equal than 8. The lowest priority is 8, and the greatest priority is 0.

> ⚠️ This is a **built-in** program, which means that it is not a process per se, thus it cannot be executed in **background** nor used with **pipes**.

### `sem`

Program that prints the OS' semaphores state.

### `wc`

Program that prints to standard output the amount of lines which it read from standard input, except the vowels.

The program ends when it reads `EOF (-1)`. Said character can be entered on the keyboard with the shortcut `Ctrl + D`.

# Piping

All of the above programs which are not built-in can be connected using the following syntax: `<program_1> | <program_2>`.

When said command is run, `<program_1>`'s stdout is redirected to `<program_2>`'s stdin.

# Background processes

All of the above programs which are not built-in can be run in background using the following syntax: `&<program>`.

# Implemented System Calls

A type 80 interrupt must be generated to execute the desired system call.

The registers mentioned below must have the following parameters for the correct execution of the system call.

The value of RAX indicates the system call to be run

The return value will be available in RAX.

| System Call | RAX | RDI | RSI | RDX |
| --- | --- | --- | --- | --- |
| read | 0x00 | `unsigned int fd` | `char * buf` | `uint64_t count` |
| write | 0x01 | `unsigned int fd` | `char * buf` | `uint64_t count` |
| exec | 0x02 | `uint64_t program` | `unsigned int argc` | `char * argv[]` |
| exit | 0x03 | `int ret_value` | `char autokill` |  |
| getpid | 0x04 |  |  |  |
| waitpid | 0x05 | `pid_t pid` |  |  |
| yield | 0x06 |  |  |  |
| block | 0x07 | `pid_t pid` |  |  |
| unblock | 0x08 | `pid_t pid` |  |  |
| kill | 0x09 | `pid_t pid` |  |  |
| nice | 0x0A | `pid_t pid` | `int new_priority` |  |
| malloc | 0x0B | `uint64_t size` |  |  |
| free | 0x0C | `uint64_t ptr` |  |  |
| sem_open | 0x0D | `char * name` | `int initial_value` |  |
| sem_close | 0x0E | `sem_t sem` |  |  |
| sem_wait | 0x0F | `sem_t sem` |  |  |
| sem_post | 0x10 | `sem_t sem` |  |  |
| pipe | 0x11 | `int fds[]` |  |  |
| dup2 | 0x12 | `int old` | `int new` |  |
| close | 0x13 | `int fd` |  |  |
| mem_info | 0x14 |  |  |  |
| pipe_info | 0x15 |  |  |  |
| sem_info | 0x16 |  |  |  |
| process_info | 0x17 |  |  |  |
| time | 0x18 | `date_s * s` |  |  |
| clear | 0x19 |  |  |  |

The system call `pipe()` receives an integer array, where it will leave the file descriptors asigned to the read and write end of the pipe.`fds[0]` refers to the read end of the pipe and `fds[1]` refers to the write end of the pipe.

The system calls `mem_info()`, `pipe_info()`, `sem_info()` and `process_info()` return a pointer to a structure with the relevant data. The user is in charge of freeing said memory.
