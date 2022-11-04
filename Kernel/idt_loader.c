#include <idt_loader.h>
#include <interrupts.h>
#include <stdint.h>
#include <defs.h>

#pragma pack(push)	
#pragma pack (1)

typedef struct {
    uint16_t offset_l, selector;
    uint8_t cero, access;
    uint16_t offset_m;
    uint32_t offset_h, other_cero;
} DESCR_INT;

#pragma pack(pop)


DESCR_INT * idt = (DESCR_INT *) 0;

static void setup_IDT_entry(int index, uint64_t offset);

void load_idt() {
    setup_IDT_entry (0x20, (uint64_t)&_irq00_handler);
    setup_IDT_entry(0x21,(uint64_t)&_irq01_handler);
    setup_IDT_entry (0x80, (uint64_t)&_syscall_handler);

    //Timer tick and keyboard.
    _master_pic_mask(0xFC);
    _slave_pic_mask(0xFF);
}

static void setup_IDT_entry(int index, uint64_t offset) {
    idt[index].selector = 0x08;
    idt[index].offset_l = offset & 0xFFFF;
    idt[index].offset_m = (offset >> 16) & 0xFFFF;
    idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
    idt[index].access = ACS_INT;
    idt[index].cero = 0;
    idt[index].other_cero = (uint64_t) 0;
}
