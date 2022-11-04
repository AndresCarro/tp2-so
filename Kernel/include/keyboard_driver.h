#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <kernel_types.h>

Pipe * keyboard_init();
void keyboard_handler();

#endif 