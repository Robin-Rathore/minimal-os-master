# MinimalOS

A minimal operating system with a basic file system and shell interface.

## Features

- Bootable from USB drive
- Simple in-memory file system
- Basic shell with commands:
  - `ls` - List directory contents
  - `cd` - Change directory
  - `mkdir` - Create directory
  - `pwd` - Print working directory
  - `clear` - Clear screen
  - `help` - Show available commands
  - `uptime` - Show system uptime
  - `echo` - Display text

## Building the OS

### Prerequisites

You need a cross-compiler for i386-elf target. Follow these steps to set it up:

1. Install build dependencies:
   \`\`\`
   sudo apt-get install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev
   \`\`\`

2. Build the cross-compiler (this may take some time):
   \`\`\`
   git clone https://github.com/cfenollosa/os-tutorial.git
   cd os-tutorial/toolchain
   ./build.sh
   \`\`\`

3. Add the cross-compiler to your PATH:
   \`\`\`
   export PATH=$PATH:$HOME/opt/cross/bin
   \`\`\`

### Building MinimalOS

1. Clone the repository:
   \`\`\`
   git clone https://github.com/yourusername/MinimalOS.git
   cd MinimalOS
   \`\`\`

2. Build the OS:
   \`\`\`
   make
   \`\`\`

3. Create a bootable USB image:
   \`\`\`
   make usb
   \`\`\`

4. Write the image to a USB drive (replace /dev/sdX with your USB device):
   \`\`\`
   sudo dd if=minimalos.img of=/dev/sdX bs=4M status=progress
   \`\`\`

## Testing in QEMU

You can test the OS in QEMU without creating a USB drive:

\`\`\`
make run
\`\`\`

## Project Structure

- `boot/` - Bootloader code
- `cpu/` - CPU-specific code (GDT, IDT, etc.)
- `drivers/` - Hardware drivers (keyboard, screen, etc.)
- `fs/` - File system implementation
- `include/` - Header files
- `kernel/` - Kernel code
- `libc/` - C library functions
- `shell/` - Shell implementation

## University Project Evaluation Guide

### Demonstration Steps

1. Boot the OS from USB:
   - Insert the USB drive into the computer
   - Restart the computer and enter the boot menu (usually F12, F2, or ESC)
   - Select the USB drive to boot from

2. Once booted, you'll see the MinimalOS shell prompt:
   \`\`\`
   MinimalOS Shell v1.0
   Type 'help' for a list of commands
   
   minimalos>
   \`\`\`

3. Demonstrate file system commands:
   \`\`\`
   minimalos> mkdir test
   Directory created: test
   
   minimalos> ls
   Contents of /:
   [DIR] test
   
   minimalos> cd test
   
   minimalos> pwd
   /test
   
   minimalos> mkdir subdir
   Directory created: subdir
   
   minimalos> ls
   Contents of /test:
   [DIR] subdir
   
   minimalos> cd ..
   
   minimalos> pwd
   /
   \`\`\`

4. Show other commands:
   \`\`\`
   minimalos> uptime
   System uptime: 42 seconds
   
   minimalos> echo Hello, MinimalOS!
   Hello, MinimalOS!
   
   minimalos> clear
   \`\`\`

### Key Points to Highlight

1. **Bootloader**: Explain how the bootloader loads the kernel into memory and switches to 32-bit protected mode.

2. **File System**: Describe the in-memory file system implementation that supports basic directory operations.

3. **Shell Interface**: Show how the shell processes commands and interacts with the file system.

4. **Device Drivers**: Mention the keyboard and screen drivers that enable user interaction.

5. **Memory Management**: Discuss the memory management utilities and how they're used in the OS.

## License

This project is licensed under the GNU General Public License v3.0 - see the LICENSE file for details.
