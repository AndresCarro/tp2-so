#ifndef CONSOLE_DRIVER_H
#define CONSOLE_DRIVER_H

#include <stdint.h>

#define ERROR_FORMAT 0x0C
#define DEFAULT_FORMAT 0x07

void delete_char();
void print_format(const char * string, uint8_t format);
void print_char_format(char character, uint8_t format);
void print(const char * string);
void print_char(char character);
void new_line();
void print_dec(uint64_t value);
void print_hex(uint64_t value);
void print_bin(uint64_t value);
void print_base(uint64_t value, uint32_t base);
void clear();
void scroll_up();

#endif