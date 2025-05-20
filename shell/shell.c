/** @file : shell.c
 *  @brief: This module implements a simple command shell.
 *
 *  Copyright (C) 2023  Your Name <your.email@example.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/

#include "minimalos/shell/shell.h"
#include "minimalos/drivers/screen.h"
#include "minimalos/fs/filesystem.h"
#include "minimalos/libc/mem.h"
#include "minimalos/cpu/timer.h"
#include <stdint.h>

#define MAX_COMMAND_LENGTH 256
#define SHELL_PROMPT "minimalos> "

// Buffer for storing the current command
static char command_buffer[MAX_COMMAND_LENGTH];
static int buffer_pos = 0;

// Flag to indicate if a command is ready to be processed
static int command_ready = 0;

// Keyboard handler variables
extern uint32_t tick; // From timer.c

// Initialize the shell
void shell_init() {
    clear_screen();
    print("MinimalOS Shell v1.0\n");
    print("Type 'help' for a list of commands\n\n");
    
    // Initialize the file system
    fs_init();
    
    // Display the prompt
    print(SHELL_PROMPT);
    
    // Clear the command buffer
    memset(command_buffer, 0, MAX_COMMAND_LENGTH);
    buffer_pos = 0;
    command_ready = 0;
}

// Process keyboard input for the shell
void shell_process_key(char key) {
    if (key == '\n') {
        // Enter key pressed, process the command
        command_buffer[buffer_pos] = '\0';
        print("\n");
        command_ready = 1;
    } else if (key == '\b') {
        // Backspace key pressed
        if (buffer_pos > 0) {
            buffer_pos--;
            command_buffer[buffer_pos] = '\0';
            // Move cursor back and clear the character
            print("\b \b");
        }
    } else {
        // Regular character
        if (buffer_pos < MAX_COMMAND_LENGTH - 1) {
            command_buffer[buffer_pos++] = key;
            // Echo the character
            char str[2] = {key, '\0'};
            print(str);
        }
    }
}

// Helper function to compare strings
static int str_compare(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) {
            return 0;
        }
        s1++;
        s2++;
    }
    return (*s1 == *s2);
}

// Helper function to check if a string starts with a prefix
static int str_starts_with(const char *str, const char *prefix) {
    while (*prefix) {
        if (*prefix != *str) {
            return 0;
        }
        prefix++;
        str++;
    }
    return 1;
}

// Process a command
void shell_process_command(const char *command) {
    // Skip leading spaces
    while (*command == ' ') {
        command++;
    }
    
    // Empty command
    if (*command == '\0') {
        return;
    }
    
    // Help command
    if (str_compare(command, "help")) {
        print("Available commands:\n");
        print("  help     - Display this help message\n");
        print("  clear    - Clear the screen\n");
        print("  ls [dir] - List directory contents\n");
        print("  cd <dir> - Change directory\n");
        print("  pwd      - Print working directory\n");
        print("  mkdir <dir> - Create a directory\n");
        print("  uptime   - Show system uptime\n");
        print("  echo <text> - Display text\n");
        return;
    }
    
    // Clear command
    if (str_compare(command, "clear")) {
        clear_screen();
        return;
    }
    
    // LS command
    if (str_starts_with(command, "ls")) {
        const char *path = command + 2;
        // Skip spaces
        while (*path == ' ') {
            path++;
        }
        
        if (*path == '\0') {
            // No path specified, list current directory
            fs_ls(NULL);
        } else {
            fs_ls(path);
        }
        return;
    }
    
    // CD command
    if (str_starts_with(command, "cd")) {
        const char *path = command + 2;
        // Skip spaces
        while (*path == ' ') {
            path++;
        }
        
        if (*path == '\0') {
            // No path specified, go to root
            fs_cd("/");
        } else {
            if (fs_cd(path) != 0) {
                print("cd: No such directory\n");
            }
        }
        return;
    }
    
    // PWD command
    if (str_compare(command, "pwd")) {
        print(fs_pwd());
        print("\n");
        return;
    }
    
    // MKDIR command
    if (str_starts_with(command, "mkdir")) {
        const char *path = command + 5;
        // Skip spaces
        while (*path == ' ') {
            path++;
        }
        
        if (*path == '\0') {
            print("mkdir: Missing operand\n");
        } else {
            fs_mkdir(path);
        }
        return;
    }
    
    // UPTIME command
    if (str_compare(command, "uptime")) {
        print("System uptime: ");
        print_dec(tick / 50); // Assuming 50Hz timer
        print(" seconds\n");
        return;
    }
    
    // ECHO command
    if (str_starts_with(command, "echo")) {
        const char *text = command + 4;
        // Skip spaces
        while (*text == ' ') {
            text++;
        }
        
        print(text);
        print("\n");
        return;
    }
    
    // Unknown command
    print("Unknown command: ");
    print(command);
    print("\n");
}

// Run the shell (main loop)
void shell_run() {
    // This function is called from the kernel main loop
    
    // Check if a command is ready to be processed
    if (command_ready) {
        shell_process_command(command_buffer);
        
        // Reset the command buffer
        memset(command_buffer, 0, MAX_COMMAND_LENGTH);
        buffer_pos = 0;
        command_ready = 0;
        
        // Display the prompt
        print(SHELL_PROMPT);
    }
}

// Function to be called from keyboard handler
void shell_handle_keypress(char key) {
    shell_process_key(key);
}
