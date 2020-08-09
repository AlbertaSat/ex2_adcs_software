/*
 * Copyright (C) 2020  University of Alberta
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef ADCS_COMMANDS_H
#define ADCS_COMMANDS_H

ADCS_returnState send(uint8_t * command, uint8_t length);

ADCS_returnState reset();

ADCS_returnState reset_log_pointer();

ADCS_returnState advance_log_pointer();

ADCS_returnState reset_boot_registers();

ADCS_returnState format_sd_card();

ADCS_returnState erase_file(File_Type file_type, uint8_t file_counter, bool erase_all);

#endif /* ADCS_COMMANDS_H */
