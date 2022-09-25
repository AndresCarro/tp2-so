
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 6

#include <naiveConsole.h>
#include <scheduler.h>

static void zero_division(uint64_t rip, uint64_t rsp, const uint64_t * backup);
static void invalid_opcode(uint64_t rip, uint64_t rsp, const uint64_t * backup);

void exceptionDispatcher(uint64_t exception, uint64_t rip, uint64_t rsp, const uint64_t * backup) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division(rip, rsp, backup);
	else if (exception == INVALID_OPCODE_EXCEPTION_ID)
	{
		invalid_opcode(rip, rsp, backup);
	}
	// If a scheduled task generated the exception, pause it.
	changeStatus(getCurrentTask());
}


static char* registers[] = {" RAX:", " RBX:", " RCX:", " RDX:", " RBP:", " RDI:", " RSI:", " R8:", " R9:", " R10:", " R11:", " R12:", " R13:", " R14:", " R15:"};

static void printRegisters(uint64_t rip, uint64_t rsp, const uint64_t * backup){
	ncNewline();
	ncPrintFormat("RIP:", ERROR_FORMAT);
	ncPrintHex(rip);
	ncPrintFormat(" RSP:", ERROR_FORMAT);
	ncPrintHex(rsp);
	for (int i = 14; i >= 0; i--)
	{
		ncPrintFormat(registers[14-i], ERROR_FORMAT);
		ncPrintHex(backup[i]);
	}
}

static void zero_division(uint64_t rip, uint64_t rsp, const uint64_t * backup) {
	// Handler para manejar excepcíon
	ncPrintFormat("ERROR: Zero division.", ERROR_FORMAT);
	printRegisters(rip, rsp, backup);
}

static void invalid_opcode(uint64_t rip, uint64_t rsp, const uint64_t * backup){
	// Handler para manejar excepcion
	ncPrintFormat("ERROR: Invalid operation code.", ERROR_FORMAT);
	printRegisters(rip, rsp, backup);
}
