// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <module_loader.h>
#include <stdint.h>
#include <console_driver.h>
#include <lib.h>

static void load_module(uint8_t ** module, void * module_target_address);
static uint32_t read_uint_32(uint8_t ** address);

void load_modules(void * payload_start, void ** module_target_address) {
	int i;
	uint8_t * current_module = (uint8_t *) payload_start;
	uint32_t module_count = read_uint_32(&current_module);

	for (i = 0; i < module_count; i++)
		load_module(&current_module, module_target_address[i]);
}

static void load_module(uint8_t ** module, void * module_target_address) {
	uint32_t module_size = read_uint_32(module);

	print("  Will copy module at 0x");
	print_hex((uint64_t) *module);
	print(" to 0x");
	print_hex((uint64_t) module_target_address);
	print(" (");
	print_dec(module_size);
	print(" bytes)");

	memcpy(module_target_address, *module, module_size);
	*module += module_size;

	print(" [Done]");
	new_line();
}

static uint32_t read_uint_32(uint8_t ** address) {
	uint32_t result = *(uint32_t*)(*address);
	*address += sizeof(uint32_t);
	return result;
}
