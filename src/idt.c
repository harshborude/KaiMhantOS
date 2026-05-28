#include "idt.h"

struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct   idt_ptr;

extern void idt_flush(uint32_t); // implemented in assembly

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags   = flags /* | 0x60 */; // for user mode
}

extern void isr0();
extern void irq1();
extern void dummy_isr();

void idt_init(void) {
    idt_ptr.limit = sizeof(struct idt_entry_struct) * 256 - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    // Get current Code Segment
    uint16_t current_cs;
    asm volatile("mov %%cs, %0" : "=r"(current_cs));

    // Set all entries to dummy_isr to prevent unhandled interrupts crashing the OS
    for(int i=0; i<256; i++) {
        idt_set_gate(i, (uint32_t)dummy_isr, current_cs, 0x8E);
    }

    // Set exception handlers
    idt_set_gate(0, (uint32_t)isr0, current_cs, 0x8E);

    // Set hardware interrupts
    idt_set_gate(33, (uint32_t)irq1, current_cs, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
}
