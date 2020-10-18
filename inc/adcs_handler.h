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
#include <stdbool.h>
#include "adcs_types.h"

//send_telecommand
ADCS_returnState adcs_telecommand(uint32_t * command, uint32_t length);


//Common Telecommands
ADCS_returnState ADCS_reset();
ADCS_returnState ADCS_reset_log_pointer();
ADCS_returnState ADCS_advance_log_pointer();
ADCS_returnState ADCS_reset_boot_registers();
ADCS_returnState ADCS_format_sd_card();
ADCS_returnState ADCS_erase_file(File_Type file_type, uint8_t file_counter, bool erase_all);
ADCS_returnState ADCS_load_file_download_block(File_Type file_type, uint8_t counter, uint32_t offset, uint16_t block_length);
ADCS_returnState ADCS_advance_file_list_read_pointer();
ADCS_returnState ADCS_initiate_file_upload(File_Upload_Dest file_dest, uint8_t block_size);
ADCS_returnState ADCS_file_upload_packet(uint16_t packet_number, char *file_bytes); //***********************************
ADCS_returnState ADCS_finalize_upload_block(File_Upload_Dest file_dest, uint32_t offset, uint16_t block_length);
ADCS_returnState ADCS_reset_upload_block();
ADCS_returnState ADCS_reset_file_list_read_pointer();
ADCS_returnState ADCS_initiate_download_burst(uint8_t msg_length, bool ignore_hole_map);


//Common Telemetry


#endif /* ADCS_HANDLER_H */
