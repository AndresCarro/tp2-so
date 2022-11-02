#include <help.h>
#include <syslib.h>

void help(){
    const char* helpstring = 
    "help                 Provides help information for commands.\n"
	"dividebyzero         Command to verify the operation of the exception routine\n"
    "                     \"Divide by zero\"\n"
    "invalidopcode        Command to verify the operation of the exception routine\n"
    "                     \"Invalid Opcode\"\n"
	"printmem             Receives as argument a pointer and performs a memory dump\n"
    "                     of 32 bytes from the address received as an argument.\n"
    "                     Format for address has to start with 0x and be followed up\n"
    "                     with the actual address written in hex.\n"
	"time                 Command to display the system day and time.\n"
    "prime                Dispalys prime numbers starting from 2.\n"
    "fibonacci            Dispalys fibonacci series numbers.\n";
    
    puts(helpstring);
}