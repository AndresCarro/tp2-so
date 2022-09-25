#include <irqDispatcher.h>


static void int_20(uint64_t * registers);
static void int_21(uint64_t * registers);

static void (*irq_handlers[])(uint64_t *) = { int_20, int_21};

void irqDispatcher(uint64_t irq, uint64_t * registers) {
	(*irq_handlers[irq])(registers);
	return;
}

static void int_20(uint64_t * registers) {
	timer_handler(registers);
}

static void int_21(uint64_t * registers) {
	keyboard_handler(registers);
}