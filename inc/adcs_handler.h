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

#ifndef ADCS_HANDLER_H
#define ADCS_HANDLER_H

#include <stdint.h>

ADCS_returnState adcs_telecommand(uint8_t * command, uint8_t length);


ADCS_returnState ADCS_reset();

ADCS_returnState ADCS_reset_log_pointer();

ADCS_returnState ADCS_advance_log_pointer();

ADCS_returnState ADCS_reset_boot_registers();

ADCS_returnState ADCS_format_sd_card();

ADCS_returnState ADCS_erase_file(File_Type file_type, uint8_t file_counter, bool erase_all);

#endif /* ADCS_HANDLER_H */
