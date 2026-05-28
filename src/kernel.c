#include "vga.h"
#include "keyboard.h"
#include "idt.h"
#include "pic.h"
#include "io.h"

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void kernel_main(void) {
    vga_init();
    
    // Set up IDT and PIC
    pic_remap(0x20, 0x28); // Map master PIC to 32, slave to 40
    idt_init();
    
    // Enable hardware interrupts!
    __asm__ __volatile__("sti");

    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_print("================================================================================\n");
    vga_print("                     KaiMhantOS : Kai Vishay Nai                                \n");
    vga_print("================================================================================\n\n");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_print("Type 'help' to see available commands, or 'divzero' to test exceptions.\n\n");

    char buffer[128];
    int pos = 0;

    vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    vga_print("KaiMhantOS> ");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    while (1) {
        // We no longer block in a tight polling loop!
        // We can just halt the CPU until an interrupt (like a keystroke) wakes it up.
        __asm__ __volatile__("hlt");

        char c = keyboard_fetch_char();
        if (c == 0) continue;

        if (c == '\n') {
            vga_putchar('\n');
            buffer[pos] = '\0';
            
            // Command parsing
            if (strcmp(buffer, "help") == 0) {
                vga_print("Available commands:\n");
                vga_print("  help    - Show this help message\n");
                vga_print("  echo    - Prints a message (Usage: echo <msg>)\n");
                vga_print("  clear   - Clear the screen\n");
                vga_print("  about   - Information about this OS\n");
                vga_print("  restart - Reboot the computer\n");
                vga_print("  shutdown- Power off the computer\n");
                vga_print("  divzero - Trigger a Divide-by-Zero Exception\n");
            } else if (strcmp(buffer, "clear") == 0) {
                vga_clear();
            } else if (strcmp(buffer, "about") == 0) {
                vga_print("KaiMhantOS v1.1\n");
                vga_print("Now running with Hardware Interrupts (IDT)!\n");
            } else if (strcmp(buffer, "divzero") == 0) {
                __asm__ __volatile__("div %0" : : "r"(0)); // Trigger divide by zero
            } else if (strcmp(buffer, "restart") == 0) {
                vga_print("Restarting system...\n");
                uint8_t good = 0x02;
                while (good & 0x02) {
                    good = inb(0x64);
                }
                outb(0x64, 0xFE);
                while(1) { __asm__ __volatile__("hlt"); } // Wait for reboot to happen
            } else if (strcmp(buffer, "shutdown") == 0) {
                vga_print("Shutting down system...\n");
                // QEMU specific ACPI shutdown (PIIX4 PM1a control register)
                outw(0x604, 0x2000); 
                while(1) { __asm__ __volatile__("hlt"); } // Wait for shutdown
            } else if (buffer[0] == 'e' && buffer[1] == 'c' && buffer[2] == 'h' && buffer[3] == 'o' && buffer[4] == ' ') {
                vga_print(&buffer[5]);
                vga_print("\n");
            } else if (pos > 0) {
                vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
                vga_print("Unknown command: ");
                vga_print(buffer);
                vga_print("\n");
                vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
            }

            pos = 0;
            vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
            vga_print("KaiMhantOS> ");
            vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

        } else if (c == '\b') {
            if (pos > 0) {
                pos--;
                vga_putchar('\b');
            }
        } else if (pos < 127) {
            buffer[pos++] = c;
            vga_putchar(c);
        }
    }
}
