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

#ifndef ADCS_TYPES_H
#define ADCS_TYPES_H

const char Telemetery_Request = 1, Telecommand = 0, TC_TR_Mask = 0x80;

enum ADCS_returnState {
  ADCS_ok,
  ADCS_send_failed
} ADCS_returnState;

typedef enum File_Type {
  telemetery_log = 2,
  JPG_image,
  BMP_image,
  index = 15
} File_Type;

typedef struct Common_Telecommand_IDs {
  uint8_t reset     = 1, // perform a reset
  reset_log_pointer       = 4, // reset pointer to log buffer
  advance_log_pointer     = 5,
  reset_boot_registers    = 6,
  format_sd_card          = 33,
  erase_file              = 108,
  load_file_download_block= 112, // fill download buffer with file contents
  advance_file_list_read_pointer = 113,
  initiate_file_upload    = 114,
  file_upload_packet      = 115,
  finalize_upload_block   = 116,
  reset_upload_block      = 117, // reset holemap for upload block
  reset_file_list_read_pointer = 118,
  initiate_download_burst = 119
} Common_Telecommand_IDs;

#endif /* ADCS_TYPES_H */
