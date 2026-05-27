#include "vga.h"
#include "keyboard.h"

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void kernel_main(void) {
    vga_init();
    
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_print("================================================================================\n");
    vga_print("                               Welcome to KaiMhantOS!                           \n");
    vga_print("================================================================================\n\n");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_print("Type 'help' to see available commands.\n\n");

    char buffer[128];
    int pos = 0;

    while (1) {
        vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
        vga_print("KaiMhant> ");
        vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        
        pos = 0;
        while (1) {
            char c = keyboard_read_char();
            if (c == 0) continue;

            if (c == '\n') {
                vga_putchar('\n');
                buffer[pos] = '\0';
                break;
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

        if (pos == 0) continue;

        // Command parsing
        if (strcmp(buffer, "help") == 0) {
            vga_print("Available commands:\n");
            vga_print("  help  - Show this help message\n");
            vga_print("  echo  - Prints a message (Usage: echo <msg>)\n");
            vga_print("  clear - Clear the screen\n");
            vga_print("  about - Information about this OS\n");
        } else if (strcmp(buffer, "clear") == 0) {
            vga_clear();
        } else if (strcmp(buffer, "about") == 0) {
            vga_print("KaiMhantOS v1.0\n");
            vga_print("A 32-bit protected mode OS built for a semester presentation.\n");
        } else if (buffer[0] == 'e' && buffer[1] == 'c' && buffer[2] == 'h' && buffer[3] == 'o' && buffer[4] == ' ') {
            vga_print(&buffer[5]);
            vga_print("\n");
        } else {
            vga_set_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
            vga_print("Unknown command: ");
            vga_print(buffer);
            vga_print("\n");
            vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        }
    }
}
