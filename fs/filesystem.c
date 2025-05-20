/** @file : filesystem.c
 *  @brief: This module implements a simple in-memory file system.
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

#include "minimalos/fs/filesystem.h"
#include "minimalos/drivers/screen.h"
#include "minimalos/libc/mem.h"
#include <stdint.h>

// File system data structures
static file_entry_t files[MAX_FILES];
static uint32_t file_count = 0;
static uint32_t current_dir = 0; // Root directory index
static char current_path[MAX_PATH_LENGTH] = "/";

// Initialize the file system
void fs_init() {
    // Clear all file entries
    memset(files, 0, sizeof(files));
    
    // Create root directory
    file_entry_t *root = &files[0];
    memset(root->name, 0, MAX_FILENAME_LENGTH);
    root->name[0] = '/';
    root->type = FILE_TYPE_DIRECTORY;
    root->size = 0;
    root->parent_dir_index = 0; // Root is its own parent
    
    file_count = 1;
    current_dir = 0;
    
    print("File system initialized.\n");
}

// Helper function to find a file by name in a directory
static int find_file(const char *name, uint32_t dir_index) {
    for (uint32_t i = 0; i < file_count; i++) {
        if (files[i].parent_dir_index == dir_index) {
            // Simple string comparison
            int match = 1;
            for (int j = 0; j < MAX_FILENAME_LENGTH; j++) {
                if (name[j] != files[i].name[j]) {
                    match = 0;
                    break;
                }
                if (name[j] == '\0') {
                    break;
                }
            }
            if (match) {
                return i;
            }
        }
    }
    return -1;
}

// Parse a path and return the directory index
int fs_parse_path(const char *path, uint32_t *dir_index) {
    if (path[0] == '/') {
        // Absolute path
        *dir_index = 0; // Start from root
        
        if (path[1] == '\0') {
            return 0; // Root directory
        }
        
        // Skip the leading '/'
        path++;
    } else {
        // Relative path
        *dir_index = current_dir;
    }
    
    char component[MAX_FILENAME_LENGTH];
    int comp_index = 0;
    
    while (*path) {
        if (*path == '/') {
            // End of component
            component[comp_index] = '\0';
            
            if (comp_index > 0) {
                // Handle ".." for parent directory
                if (component[0] == '.' && component[1] == '.' && component[2] == '\0') {
                    *dir_index = files[*dir_index].parent_dir_index;
                } else {
                    // Find the directory
                    int file_index = find_file(component, *dir_index);
                    if (file_index == -1) {
                        print("Directory not found: ");
                        print(component);
                        print("\n");
                        return -1;
                    }
                    
                    if (files[file_index].type != FILE_TYPE_DIRECTORY) {
                        print("Not a directory: ");
                        print(component);
                        print("\n");
                        return -1;
                    }
                    
                    *dir_index = file_index;
                }
            }
            
            comp_index = 0;
        } else {
            if (comp_index < MAX_FILENAME_LENGTH - 1) {
                component[comp_index++] = *path;
            }
        }
        
        path++;
    }
    
    // Handle the last component if any
    if (comp_index > 0) {
        component[comp_index] = '\0';
        
        // Handle ".." for parent directory
        if (component[0] == '.' && component[1] == '.' && component[2] == '\0') {
            *dir_index = files[*dir_index].parent_dir_index;
        } else {
            // Find the directory
            int file_index = find_file(component, *dir_index);
            if (file_index == -1) {
                return -1; // Not found, but this might be ok for mkdir
            }
            
            if (files[file_index].type != FILE_TYPE_DIRECTORY) {
                print("Not a directory: ");
                print(component);
                print("\n");
                return -1;
            }
            
            *dir_index = file_index;
        }
    }
    
    return 0;
}

// Create a new directory
int fs_mkdir(const char *path) {
    if (file_count >= MAX_FILES) {
        print("Error: Maximum number of files reached\n");
        return -1;
    }
    
    uint32_t parent_dir = current_dir;
    char dir_name[MAX_FILENAME_LENGTH];
    int name_index = 0;
    
    // Extract the directory name from the path
    const char *last_slash = path;
    for (const char *p = path; *p; p++) {
        if (*p == '/') {
            last_slash = p + 1;
        }
    }
    
    // Copy the directory name
    for (const char *p = last_slash; *p && name_index < MAX_FILENAME_LENGTH - 1; p++) {
        dir_name[name_index++] = *p;
    }
    dir_name[name_index] = '\0';
    
    // If there's a path before the directory name, navigate to it
    if (last_slash > path) {
        char parent_path[MAX_PATH_LENGTH];
        int path_len = last_slash - path;
        if (path_len > 0) {
            for (int i = 0; i < path_len; i++) {
                parent_path[i] = path[i];
            }
            parent_path[path_len] = '\0';
            
            if (fs_parse_path(parent_path, &parent_dir) != 0) {
                return -1;
            }
        }
    }
    
    // Check if directory already exists
    if (find_file(dir_name, parent_dir) != -1) {
        print("Error: Directory already exists\n");
        return -1;
    }
    
    // Create the directory
    file_entry_t *new_dir = &files[file_count];
    memset(new_dir, 0, sizeof(file_entry_t));
    
    // Copy the name
    for (int i = 0; i < name_index; i++) {
        new_dir->name[i] = dir_name[i];
    }
    
    new_dir->type = FILE_TYPE_DIRECTORY;
    new_dir->parent_dir_index = parent_dir;
    
    file_count++;
    
    print("Directory created: ");
    print(dir_name);
    print("\n");
    
    return 0;
}

// List contents of a directory
void fs_ls(const char *path) {
    uint32_t dir_index = current_dir;
    
    // If a path is provided, navigate to it
    if (path && *path) {
        if (fs_parse_path(path, &dir_index) != 0) {
            return;
        }
    }
    
    print("Contents of ");
    if (dir_index == 0) {
        print("/");
    } else {
        print(files[dir_index].name);
    }
    print(":\n");
    
    int found = 0;
    
    // List all files in the directory
    for (uint32_t i = 0; i < file_count; i++) {
        if (files[i].parent_dir_index == dir_index && i != dir_index) {
            if (files[i].type == FILE_TYPE_DIRECTORY) {
                print("[DIR] ");
            } else {
                print("[FILE] ");
            }
            print(files[i].name);
            print("\n");
            found = 1;
        }
    }
    
    if (!found) {
        print("(empty)\n");
    }
}

// Change current directory
int fs_cd(const char *path) {
    uint32_t new_dir = current_dir;
    
    if (fs_parse_path(path, &new_dir) != 0) {
        return -1;
    }
    
    current_dir = new_dir;
    
    // Update current path
    if (new_dir == 0) {
        current_path[0] = '/';
        current_path[1] = '\0';
    } else {
        // Build the full path
        char temp_path[MAX_PATH_LENGTH];
        uint32_t dir = new_dir;
        int path_len = 0;
        
        // Traverse up to build the path
        while (dir != 0) {
            // Add this directory name to the beginning of the path
            int name_len = 0;
            while (files[dir].name[name_len]) {
                name_len++;
            }
            
            // Shift existing path to make room
            for (int i = path_len; i >= 0; i--) {
                temp_path[i + name_len + 1] = temp_path[i];
            }
            
            // Copy the directory name
            for (int i = 0; i < name_len; i++) {
                temp_path[i] = files[dir].name[i];
            }
            temp_path[name_len] = '/';
            path_len += name_len + 1;
            
            dir = files[dir].parent_dir_index;
        }
        
        // Copy to current_path with leading '/'
        current_path[0] = '/';
        for (int i = 0; i < path_len; i++) {
            current_path[i + 1] = temp_path[i];
        }
        current_path[path_len + 1] = '\0';
    }
    
    return 0;
}

// Get current directory path
const char* fs_pwd() {
    return current_path;
}
