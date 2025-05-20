/** @file : kernel.c
 *  @brief: This module supplies the kernel functions for the operating
 * 			system.
 *
 *  Copyright (C) 2020  Suraj Upadhyay <usuraj35@gmail.com>
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

#include "minimalos/drivers/screen.h"
#include "minimalos/drivers/keyboard.h"
#include "minimalos/cpu/dt.h"
#include "minimalos/cpu/timer.h"
#include "minimalos/shell/shell.h"
#include "minimalos/fs/filesystem.h"

void kernel_main(void)
{
	clear_screen();
	init_dt();
	init_timer(50);
	init_keyboard();
	
	// Initialize the shell
	shell_init();
	
	// Main kernel loop
	while (1) {
		shell_run();
	}
}
