#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <pipe.h>
#include <idtLoader.h>
#include <naiveConsole.h>
#include <lib.h>
#include <scheduler.h>
#include <inforeg.h>

Pipe * keyboard_init();
void keyboard_handler(uint64_t * registers);

#endif 