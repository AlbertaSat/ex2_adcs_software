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

#define ADCS_Telemetery_Request 1;
#define ADCS_Telecommand 0;
#define ADCS_TC_TR_Mask 0x80;

#define ADCS_MAGIC_NUMBER 0x5A;
#define ADCS_ESC_CHAR 0x1F;
#define ADCS_SOM 0x7F;
#define ADCS_EOM 0xFF;

typedef enum ADCS_returnState {
  ADCS_OK = 0,
  ADCS_INVALID_ID = 1,
  ADCS_INCORRECT_LENGTH = 2,
  ADCS_INVALID_PARAMETERS = 3,
  ADCS_CRC_ERROR = 4
} ADCS_returnState;

typedef enum File_Type {
  FILE_TYPE_TELEMETERY_LOG = 2,
  FILE_TYPE_JPG_IMAGE = 3,
  FILE_TYPE_BMP_IMAGE = 4,
  FILE_TYPE_INDEX = 15
} File_Type;

typedef enum File_Upload_Dest {
  EEPROM = 2,
  FLASH_PROGRAM_1 = 3,
  FLASH_PROGRAM_2 = 4,
  FLASH_PROGRAM_3 = 5,
  FLASH_PROGRAM_4 = 6,
  FLASH_PROGRAM_5 = 7,
  FLASH_PROGRAM_6 = 8,
  FLASH_PROGRAM_7 = 9,
  SD_USER_FILE_1 = 10,
  SD_USER_FILE_2 = 11,
  SD_USER_FILE_3 = 12,
  SD_USER_FILE_4 = 13,
  SD_USER_FILE_5 = 14,
  SD_USER_FILE_6 = 15,
  SD_USER_FILE_7 = 16,
  SD_USER_FILE_8 = 17
} File_Upload_Dest;

typedef enum Common_Telecommand_IDs {
  RESET_ID = 1,              // perform a reset
  RESET_LOG_POINTER_ID = 4,  // reset pointer to log buffer
  ADVANCE_LOG_POINTER_ID = 5,
  RESET_BOOT_REGISTERS_ID = 6,
  FORMAT_SD_CARD_ID = 33,
  ERASE_FILE_ID = 108,
  LOAD_FILE_DOWNLOAD_BLOCK_ID = 112,  // fill download buffer with file contents
  ADVANCE_FILE_LIST_READ_POINTER_ID = 113,
  INITIATE_FILE_UPLOAD_ID = 114,
  FILE_UPLOAD_PACKET_ID = 115,
  FINALIZE_UPLOAD_BLOCK_ID = 116,
  RESET_UPLOAD_BLOCK_ID = 117,  // reset holemap for upload block
  RESET_FILE_LIST_READ_POINTER_ID = 118,
  INITIATE_DOWNLOAD_BURST_ID = 119
} Common_Telecommand_IDs;

typedef enum BootLoader_Telecommand_IDs {
  CLEAR_ERR_FLAGS_ID = 7,
  SET_BOOT_INDEX_ID = 100,
  RUN_SELECTED_PROGRAM_ID,
  READ_PROGRAM_INFO_ID,
  COPY_PROGRAM_INTERNAL_FLASH_ID
} BootLoader_Telecommand_IDs;

typedef enum BootLoader_Telemetry_IDs {
  GET_BOOTLOADER_STATE_ID = 132,
  GET_PROGRAM_INFO_ID = 232,
  COPY_INTERNAL_FLASH_PROGRESS_ID = 233
} BootLoader_Telemetry_IDs;

typedef enum ACP_Telecommand_IDs {
  DEPLOY_MAGNETOMETER_BOOM_ID =
      7,  //* The ID is the same is CLEAR_ERR_FLAGS! how could it be?
  ADCS_RUN_MODE_ID = 10,
  CLEAR_LATCHED_ERRS_ID = 12,
  SET_ATT_CONTROL_MODE_ID = 13,  // attitude
  SET_ATT_ESTIMATE_MODE_ID = 14,
  TRIGGER_ADCS_LOOP_ID = 18,
  TRIGGER_ADCS_LOOP_SIM_ID = 19,
  ASGP4_RUN_MODE_ID = 31,
  ASGP4_TRIGGER_ID = 32,
  SET_MTM_OP_MODE_ID = 56,
  CNV2JPG_ID = 57,
  SAVE_IMG_ID = 80,
  SET_MAGNETORQUER_OUTPUT_ID = 16,
  SET_WHEEL_SPEED_ID = 17,
  SAVE_CONFIG_ID = 63,
  SAVE_ORBIT_PARAMS = 64,
} ACP_Telecommand_IDs;

typedef enum ACP_Telemetry_IDs {
  // ADCS state
  ADCS_STATE = 190,  //* Includes 132, 146:150, 201, 218-219, 224
  // General
  JPG_CNV_PROGRESS_ID = 133,
  CUBEACP_STATE_FLAGS_ID = 135,
  ADCS_EXE_TIMES_ID = 196,
  ACP_EXE_STATE_ID = 220,
  IMG_CAPTURE_SAVE_OP_STAT = 233,
  // ADCS measurements
  ADCS_MEASUREMENTS_ID = 191,  //* Includes 151:156, 181:186
  // Actuator
  ACTUATOR_ID = 192,  //* Includes 157,158
  // Estimation
  ESTIMATION_ID = 193,  //* Includes 159:165
  ASGP4_TLEs_ID = 228,
  // Raw sensor
  RAW_SENSOR_MEASUREMENTS_ID = 194,  //* Includes 166:170, 216
  RAW_GPS_MEASUREMENTS_ID = 210,     //* Includes 176:180
  RAW_STAR_TRACKER_ID = 211,         //* Includes 187:189, 212:214, 229:231
  MTM2_MEASUREMENTS_ID = 215,
  // Power
  POWER_TEMP_ID = 195,  //* Includes 171:175, 198, 232
} ACP_Telemetry_IDs;

#endif /* ADCS_TYPES_H */
