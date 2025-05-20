/** @file : shell.h
 *  @brief: This module defines a simple shell interface.
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

#ifndef SHELL_H
#define SHELL_H

// Initialize the shell
void shell_init();

// Run the shell (main loop)
void shell_run();

// Process a command
void shell_process_command(const char *command);

#endif /* !defined(SHELL_H). */
