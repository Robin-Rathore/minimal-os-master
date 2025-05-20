/** @file : filesystem.h
 *  @brief: This module defines a simple file system interface.
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

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdint.h>

#define MAX_FILENAME_LENGTH 32
#define MAX_FILES 64
#define MAX_DIRECTORIES 32
#define MAX_PATH_LENGTH 256

typedef enum {
    FILE_TYPE_REGULAR,
    FILE_TYPE_DIRECTORY
} file_type_t;

typedef struct {
    char name[MAX_FILENAME_LENGTH];
    file_type_t type;
    uint32_t size;
    uint32_t parent_dir_index; // Index of parent directory
    uint8_t content[512];      // Simple content storage for demo
} file_entry_t;

// Initialize the file system
void fs_init();

// Create a new directory
int fs_mkdir(const char *path);

// List contents of a directory
void fs_ls(const char *path);

// Change current directory
int fs_cd(const char *path);

// Get current directory path
const char* fs_pwd();

// Helper function to parse paths
int fs_parse_path(const char *path, uint32_t *dir_index);

#endif /* !defined(FILESYSTEM_H). */
