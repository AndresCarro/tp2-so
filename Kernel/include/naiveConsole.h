#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>
#include <lib.h>
#include <dates.h>

#define ERROR_FORMAT 0x0C

void ncDeleteChar();
void ncPrintFormat(const char* string,uint8_t format);
void ncPrintCharFormat(char character,uint8_t format);
void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
void ncClearWindow(uint8_t windowToCLear);
uint8_t ncWindows(uint8_t amount);
uint8_t ncCurrentWindow(uint8_t select);
void scrollUp();

#endif