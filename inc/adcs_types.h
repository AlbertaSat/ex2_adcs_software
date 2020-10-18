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

#define ADCS_Telemetery_Request  1; 
#define ADCS_Telecommand  0;
#define ADCS_TC_TR_Mask  0x80;

#define ADCS_MAGIC_NUMBER  0x5A;
#define ADCS_ESC_CHAR      0x1F;
#define ADCS_SOM           0x7F;
#define ADCS_EOM           0xFF;

typedef enum ADCS_returnState {
  ADCS_OK                 = 0,
  ADCS_INVALID_ID         = 1,
  ADCS_INCORRECT_LENGTH   = 2,
  ADCS_INVALID_PARAMETERS = 3,
  ADCS_CRC_ERROR          = 4
} ADCS_returnState;

typedef enum File_Type {
  FILE_TYPE_TELEMETERY_LOG    = 2,
  FILE_TYPE_JPG_IMAGE         = 3,
  FILE_TYPE_BMP_IMAGE         = 4,
  FILE_TYPE_INDEX             = 15
} File_Type;

typedef enum Common_Telecommand_IDs {
  RESET_ID                           = 1,    // perform a reset
  RESET_LOG_POINTER_ID               = 4,    // reset pointer to log buffer
  ADVANCE_LOG_POINTER_ID             = 5,    
  RESET_BOOT_REGISTERS_ID            = 6,
  FORMAT_SD_CARD_ID                  = 33,
  ERASE_FILE_ID                      = 108,
  LOAD_FILE_DOWNLOAD_BLOCK_ID        = 112,  // fill download buffer with file contents
  ADVANCE_FILE_LIST_READ_POINTER_ID  = 113,
  INITIATE_FILE_UPLOAD_ID            = 114,
  FILE_UPLOAD_PACKET_ID              = 115,
  FINALIZE_UPLOAD_BLOCK_ID           = 116,
  RESET_UPLOAD_BLOCK_ID              = 117,  // reset holemap for upload block
  RESET_FILE_LIST_READ_POINTER_ID    = 118,
  INITIATE_DOWNLOAD_BURST_ID         = 119
} Common_Telecommand_IDs;

#endif /* ADCS_TYPES_H */
