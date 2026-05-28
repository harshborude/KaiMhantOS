#include "isr.h"
#include "vga.h"
#include "keyboard.h"
#include "io.h"

void isr0_handler(void) {
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_RED);
    vga_print("\nEXCEPTION: DIVIDE BY ZERO\n");
    // Halt execution since we can't recover easily without process isolation
    while(1) { asm volatile("hlt"); }
}

void irq1_handler(void) {
    keyboard_handle_interrupt();
    // Send End of Interrupt (EOI) to master PIC
    outb(0x20, 0x20);
}
