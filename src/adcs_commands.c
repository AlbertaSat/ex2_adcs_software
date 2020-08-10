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
/**
 * @file adcs_commands.c
 * @author Andrew Rooney
 * @date 2020-08-09
 */

#include <adcs_commands.h>
#include <adcs_types.h>
#include <adcs_io.h>

Common_Telecommand_IDs common_telecommand_IDs;

ADCS_returnState adcs_telecommand(uint8_t * command, uint8_t length) {
  return ADCS_ok;
}

ADCS_returnState reset() {
  uint8_t command[2];
  command[0] = common_telecommand_IDs.reset;
  command[1] = 0x5A; // Magic number
  return adcs_telecommand(&command, 2);
}

ADCS_returnState reset_log_pointer() {
  uint8_t command = common_telecommand_IDs.reset_log_pointer;
  return adcs_telecommand(&command, 1);
}

ADCS_returnState advance_log_pointer() {
  uint8_t command = common_telecommand_IDs.advance_log_pointer;
  return adcs_telecommand(&command, 1);
}

ADCS_returnState reset_boot_registers() {
  uint8_t command = common_telecommand_IDs.reset_boot_registers;
  return adcs_telecommand(&command, 1);
}

ADCS_returnState format_sd_card() {
  uint8_t command[2];
  command[0] = common_telecommand_IDs.format_sd_card;
  command[1] = 0x5A; // magic number
  return adcs_telecommand(&command, 2);
}

ADCS_returnState erase_file(File_Type file_type, uint8_t file_counter, bool erase_all) {
  uint8_t command[4];
  command[0] = common_telecommand_IDs.erase_file;
  command[1] = file_type;
  command[2] = file_counter;
  command[3] = erase_all;
  return adcs_telecommand(&command, 4);
}
