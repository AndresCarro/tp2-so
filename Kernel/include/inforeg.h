#ifndef INFOREG_H
#define INFOREG_h

#include <stdint.h>
#include <defs.h>

void setRegisters(uint64_t * registers);

int getRegisters(uint64_t * output); 

#endif
