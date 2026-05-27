#include "keyboard.h"
#include "io.h"

// Scancode to ASCII mapping (US QWERTY, simple layout)
static const char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

char keyboard_read_char(void) {
    while (1) {
        // Wait until PS/2 controller has data
        if (inb(0x64) & 0x01) {
            uint8_t scancode = inb(0x60);
            
            // Ignore key releases (high bit set)
            if (!(scancode & 0x80)) {
                if (scancode < sizeof(scancode_to_ascii)) {
                    return scancode_to_ascii[scancode];
                }
            }
        }
    }
}
