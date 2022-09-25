#include <scheduler.h>

#define DEFAULT_CURRENT -1
#define TASK_ARR_SIZE 2
#define DEFAULT_FLAGS 0x202 // Interrupts and Intel reserved bits: 1000000010b = 0x202

#pragma pack(push)  /* Push de la alineación actual */
#pragma pack (1)    /* Alinear las siguientes estructuras a 1 byte */

/* Contexto de cada task */
typedef struct {
    uint64_t registers[REG_AMOUNT]; // Registers: R15, ..., RBX, RAX
    uint64_t rsp, rip, rflags;
    uint64_t buRIP;
    uint8_t window, active, finished;
} TASK_CONTEXT;

#pragma pack(pop)   /* Reestablece la alineación actual */


static TASK_CONTEXT origin;     //Backup del caller.
static TASK_CONTEXT tasks[TASK_ARR_SIZE];    //Task array

//Amount of tasks currently running
//If amount == -1, then tasks where terminated and original context must be reloaded
static int amount = 0;
static int current = DEFAULT_CURRENT;         //Currently running task

static void * const userlandAddress = (void*)0x700000;
static void * const stepping = (void*)0x100000;

static void activateTask(unsigned int task);
static void pauseTask(unsigned int task);
static void saveContext(TASK_CONTEXT * context, uint64_t * registers);
static void loadContext(uint64_t * registers, TASK_CONTEXT * context);

void loadTasks(int (*program1)(), int (*program2)(), uint64_t * registers){
    ncWindows(2);
    tasks[0].rip = (uint64_t) program1;
    tasks[1].rip = (uint64_t) program2;
    for (int i = 0; i < TASK_ARR_SIZE; i++)
    {
        tasks[i].rsp = (uint64_t) userlandAddress + (uint64_t) stepping * i;
        //Return address
        *((uint64_t*)tasks[i].rsp) = (uint64_t) _defaultExit;
        tasks[i].window = i;
        tasks[i].active = 1;
        tasks[i].finished = 0;
        tasks[i].rflags = DEFAULT_FLAGS;
    }
    amount = 2;

    //Save origin context
    saveContext(&origin, registers);
    nextTask(registers);
}

void changeStatus(unsigned int taskNum){
    if (amount <= 0)    // Do nothing if no tasks are running or they were terminated.
        return;
    int i = taskNum % TASK_ARR_SIZE;
    if (tasks[i].active == 1)
        pauseTask(i);
    else
        activateTask(i);
}

static void pauseTask(unsigned int task){
    if (!(tasks[task].rip >= (uint64_t) haltcpu && tasks[task].rip <= (uint64_t) _endhaltcpu))
        tasks[task].buRIP = tasks[task].rip;
    tasks[task].rip = (uint64_t)haltcpu;
    tasks[task].active = 0;
}

static void activateTask(unsigned int task){
    tasks[task].rip = tasks[task].buRIP;
    tasks[task].active = 1;
}

void nextTask(uint64_t * registers){
    if (amount == 0)
        return;
    if (amount == -1){
        amount = 0;
        ncWindows(1);
        loadContext(registers,&origin);
        return;
    }
    int next = (current+1) % TASK_ARR_SIZE;

    if (tasks[next].finished == 1){
        if (tasks[current].finished == 1){
            ncWindows(1);
            loadContext(registers, &origin);
            return;
        }
        if (tasks[current].active == 0){
            if (!(tasks[current].rip >= (uint64_t) haltcpu && tasks[current].rip <= (uint64_t) _endhaltcpu))
                tasks[current].buRIP = registers[RIP_POS];
            saveContext(&tasks[current], registers);
            tasks[current].rip = (uint64_t) haltcpu;
            loadContext(registers, &tasks[current]);
        } else {
            if ((registers[RIP_POS] >= (uint64_t) haltcpu && registers[RIP_POS] <= (uint64_t) _endhaltcpu)){
                saveContext(&tasks[current], registers);
                tasks[current].rip = tasks[current].buRIP;
                loadContext(registers, &tasks[current]);
            }
        }
        return;
    }

    if (current != DEFAULT_CURRENT && tasks[current].finished != 1)
        saveContext(&(tasks[current]), registers);
    current = next;
    ncCurrentWindow(tasks[current].window);
    loadContext(registers, &(tasks[current]));
}

void exitTask(int retValue, uint64_t * registers){
    if (amount == 0)
        return;
    tasks[current].finished = 1;
    amount--;
    ncNewline();
    ncPrint("Program returned ");
    ncPrintDec(retValue);
    if (amount == 0){
        current = DEFAULT_CURRENT;
        ncWindows(1);
        loadContext(registers, &origin);
    } else
        nextTask(registers);
}

static void saveContext(TASK_CONTEXT * context, uint64_t * registers){
    for (int i = 0; i < REG_AMOUNT; i++)
    {
        context->registers[i] = registers[i];
    }
    context->rip = registers[RIP_POS];
    context->rflags = registers[RFLAGS_POS];
    context->rsp = registers[RSP_POS];
}

static void loadContext(uint64_t * registers, TASK_CONTEXT * context){
    for (int i = 0; i < REG_AMOUNT; i++)
    {
        registers[i] = context->registers[i];
    }
    registers[RIP_POS] = context->rip;
    registers[RFLAGS_POS] = context->rflags;
    registers[RSP_POS] = context->rsp;
}

void terminateTasks(){
    // Do nothing if no tasks are running, or they were already terminated.
    if (amount <= 0)
        return;
    for (int i = 0; i < TASK_ARR_SIZE; i++)
    {
        tasks[i].finished = 1;
    } 
    current = DEFAULT_CURRENT;
    amount = -1;
}

int getCurrentTask(){
    return current;
}
