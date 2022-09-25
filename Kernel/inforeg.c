#include <inforeg.h>

static uint64_t genRegs[REG_AMOUNT+3] = {0};
static uint8_t set = 0;

void setRegisters(uint64_t * registers){
    int i = 0;
    for (; i < REG_AMOUNT; i++)
    {
        genRegs[i] = registers[i];
    }
    genRegs[i++] = registers[RIP_POS];
    genRegs[i++] = registers[RFLAGS_POS];
    genRegs[i] = registers[RSP_POS];
    set = 1;
}

int getRegisters(uint64_t * output){
    if (! set)
        return 0;
    
    for (int i = 0; i < REG_AMOUNT+3; i++)
    {
        output[i] = genRegs[i];
    }
    return 1;
}
