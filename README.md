# KaiMhantOS

A simple 32-bit protected mode Operating System built for learning Operating Systems.

<img width="717" height="337" alt="image" src="https://github.com/user-attachments/assets/f62856d4-571a-4952-9577-896ca3e3c86d" />


## Features
- **32-Bit Protected Mode**: Boots directly into x86 32-bit mode via GRUB Multiboot.
- **C Kernel**: The core of the OS is written in standard C.
- **VGA Text Mode**: Custom driver for colored text output and scrolling on a standard 80x25 terminal.
- **PS/2 Keyboard Driver**: Real-time keyboard input.
- **Interactive Shell**: A basic command prompt supporting simple commands (`help`, `clear`, `echo`, `about`).

## Building and Running

You will need a Linux environment (WSL on Windows works perfectly) with the following tools installed:
```bash
sudo apt-get install build-essential nasm qemu-system-x86 grub-pc-bin xorriso
```

To compile the OS and generate the bootable ISO:
```bash
make iso
```

To run the OS in an emulator (QEMU):
```bash
make run
```

