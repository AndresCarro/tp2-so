// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <lib.h>
#include <module_loader.h>
#include <idt_loader.h>
#include <memory_manager.h>
#include <scheduler.h>
#include <semaphore.h>
#include <interrupts.h>
#include <keyboard_driver.h>
#include <console_driver.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static uint64_t const sample_code_module_address = 0x400000;
static uint64_t const sample_data_module_address = 0x500000;

void clear_BSS(void * bss_address, uint64_t bss_size) {
	memset(bss_address, 0, bss_size);
}

void * get_stack_base() {
	return (void*)((uint64_t)&endOfKernel + PageSize * 8 - sizeof(uint64_t));
}

void * initialize_kernel_binary() {
	void * module_addresses[] = {(void *) sample_code_module_address, (void *) sample_data_module_address};

	load_modules(&endOfKernelBinary, module_addresses);

	clear_BSS(&bss, &endOfKernel - &bss);

	return get_stack_base();
}

int main() {
	_cli();

    clear();
	load_idt();
	
	memory_manager_start((void *) 0xF00000, 0x2000000 - 0xF00000);
	Pipe * stdin = keyboard_init();
	scheduler_init(stdin);

	char * name = "Userland";
	char * argv[] = {name};
	create_process(sample_code_module_address, 1, argv);

    _sti();

    _hlt();

	return 0;
}
