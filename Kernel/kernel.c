#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <memory_manager.h>
#include <scheduler2.h>
#include <semaphore.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static uint64_t const sampleCodeModuleAddress = 0x400000;
static uint64_t const sampleDataModuleAddress = 0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary() {
	void * moduleAddresses[] = {
		(void *) sampleCodeModuleAddress,
		(void *) sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

// void dummy2() {
// 	sem_t sem = sem_open("prueba", 0);
// 	// _cli();
// 	sem_wait(sem);
// 	// _sti();
// 	while(1) {
// 		ncPrint("C");
// 		_hlt();
// 	}
// }

// void dummy3() {
// 	sem_t sem = sem_open("prueba", 0);
// 	for (int i = 0; i < 100; i++) {
// 		ncPrint("B");
// 		ncPrintDec(i);
// 		if (i == 50) {
// 			// _cli();
// 			sem_post(sem);
// 			// _sti();
// 		}
// 		_hlt();
// 	}
// }

int main() {
	_cli();

    ncClear();
	load_idt();
	memory_manager_start((void *) 0xF00000, 0x2000000 - 0xF00000);
	scheduler_init();
	create_process(sampleCodeModuleAddress, 0, NULL);
	// create_process(dummy3, 0, NULL);
	// create_process(dummy2, 0, NULL);
    _sti();

    _hlt();

	ncPrint("[Finished]");
	return 0;
}
