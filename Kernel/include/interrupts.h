#ifndef INTERRUPS_H_
#define INTERRUPS_H_

#include <stdint.h>

extern void _irq00_handler();
extern void _irq01_handler();
extern void _irq02_handler();
extern void _irq03_handler();
extern void _irq04_handler();
extern void _irq05_handler();

extern void _cli();
extern void _sti();
extern void _hlt();

extern void _master_pic_mask(uint8_t mask);
extern void _slave_pic_mask(uint8_t mask);

extern uint64_t _syscall_handler();

#endif
