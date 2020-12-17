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
 * @author Andrew Rooney, Vasu Gupta, Arash Yazdani
 * @date 2020-08-09
 */

#include "adcs_handler.h"

#include <string.h>

#include "adcs_io.h"
#include "adcs_types.h"

#define USE_UART
//#define USE_I2C

/*************************** General functions ***************************/
/**
 * @brief
 *		send and receive acknowledgment via selected data protocol (i2c,
 *SPI, UART)
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState adcs_telecommand(uint8_t* command, uint32_t length) {
  ADCS_returnState ack = ADCS_OK;

#ifdef USE_UART
  ack = send_uart_telecommand(command, length);
#elif USE_I2C
  ack = send_i2c_telecommand(command, length);
#endif

  return ack;
}

/**
 * @brief
 *		request telemetry and receive data via selected data protocol
 *(i2c, SPI, UART)
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState adcs_telemetry(uint8_t TM_ID, uint8_t* reply,
                                uint32_t length) {
  ADCS_returnState ack = ADCS_OK;
#ifdef USE_UART
  ack = request_uart_telemetry(TM_ID, reply, length);
#elif USE_I2C
  ack = request_i2c_telemetry(TM_ID, reply, length);
#endif

  return ack;
}

/**
 * @brief
 * 		append two bytes of a int16_t
 * @param b1 LSB, b2 MSB
 * @return
 * 		the appended int16_t
 */
int16_t uint82int16(uint8_t b1, uint8_t b2) {
  int16_t b;
  if (b2 >> 7) {  // negative
    b = -1 * ((~((b2 << 8) | b1)) + 1);
  } else
    b = (b2 << 8) | b1;  // positive
  return b;
}

/**
 * @brief
 * 		append four bytes of a int32_t
 * @param address
 * 		the address of the 4 bytes
 * @return
 * 		the appended int32_t
 */
int32_t uint82int32(uint8_t* address) {
  int32_t b;
  if (*(address + 3) >> 7) {  // negative
    b = -1 * ((~((*(address + 3) << 24) | (*(address + 2) << 16) |
                 (*(address + 1) << 8) | *address)) +
              1);
  } else
    b = (*(address + 3) << 24) | (*(address + 2) << 16) |
        (*(address + 1) << 8) | *address;  // positive
  return b;
}

/**
 * @brief
 * 		append two bytes of a uint16_t to a float
 * @param b1 LSB, b2 MSB
 * @return
 * 		the appended float
 */
uint16_t uint82uint16(uint8_t b1, uint8_t b2) {  //* improve
  uint16_t b;
  b = (b2 << 8) | b1;
  return b;
}

/**
 * @brief
 * 		a supplementary function for many ACP TMs
 * @detail
 * 		Converts the correct value from telemetry bytes with the
 * coefficient factor
 * @param measurement
 * 		the measured parameter
 * @param address
 * 		the position in the telemetry frame where the data is located
 * @param coef
 * 		formatted value = rawval * coef;
 */
void get_xyz(xyz* measurement, uint8_t* address, float coef) {  //*check
  measurement->x = coef * uint82int16(*address, *(address + 1));
  measurement->y = coef * uint82int16(*(address + 2), *(address + 3));
  measurement->z = coef * uint82int16(*(address + 4), *(address + 5));
}

/**
 * @brief
 * 		a supplementary function for many ACP TMs
 * @detail
 * 		Converts the correct value from telemetry bytes
 * @param measurement
 * 		the measured parameter
 * @param address
 * 		the position in the telemetry frame where the data is located
 */
void get_xyz16(xyz16* measurement, uint8_t* address) {
  measurement->x = uint82int16(*address, *(address + 1));
  measurement->y = uint82int16(*(address + 2), *(address + 3));
  measurement->z = uint82int16(*(address + 4), *(address + 5));
}

/*************************** Commond TCs ***************************/
/**
 * @brief
 * 		Perform a reset.
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_reset(void) {
  uint8_t command[2];
  command[0] = RESET_ID;
  command[1] = ADCS_MAGIC_NUMBER;  // Magic number 0x5A
  return adcs_telecommand(command, 2);
}

/**
 * @brief
 * 		Reset pointer to log buffer (from where LastLogEvent TLM is
 returned)
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_reset_log_pointer(void) {
  uint8_t command = RESET_LOG_POINTER_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Advance pointer to log buffer (from where LastLogEvent TLM is
 returned)
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_advance_log_pointer(void) {
  uint8_t command = ADVANCE_LOG_POINTER_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Reset Boot Registers
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_reset_boot_registers(void) {
  uint8_t command = RESET_BOOT_REGISTERS_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Format SD Card
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_format_sd_card(void) {
  uint8_t command[2];
  command[0] = FORMAT_SD_CARD_ID;
  command[1] = ADCS_MAGIC_NUMBER;  // magic number 0x5A
  return adcs_telecommand(command, 2);
}

/**
 * @brief
 * 		Erase file
 * @param file_type 	//* Not sure if a new type is necessary
 * 		Accepted parameters: FILE_TYPE_TELEMETERY_LOG = 2,
 * FILE_TYPE_JPG_IMAGE = 3, FILE_TYPE_BMP_IMAGE = 4, FILE_TYPE_INDEX = 15
 * @param file_counter
 * @param erase_all
 * 		if erase all
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_erase_file(File_Type file_type, uint8_t file_counter,
                                 bool erase_all) {
  uint8_t command[4];
  command[0] = ERASE_FILE_ID;
  command[1] = file_type;
  command[2] = file_counter;
  command[3] = erase_all;
  return adcs_telecommand(command, 4);
}

/**
 * @brief
 * 		Fill download with file contents
 * @param file_type
 * 		Accepted parameters: FILE_TYPE_TELEMETERY_LOG = 2,
 * FILE_TYPE_JPG_IMAGE = 3, FILE_TYPE_BMP_IMAGE = 4, FILE_TYPE_INDEX = 15
 * @param counter
 * @param offset
 * @param block_length
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_load_file_download_block(File_Type file_type,
                                               uint8_t counter, uint32_t offset,
                                               uint16_t block_length) {
  uint8_t command[9];
  command[0] = LOAD_FILE_DOWNLOAD_BLOCK_ID;
  command[1] = file_type;
  command[2] = counter;
  memcpy(&command[3], &offset, 4);
  command[7] = block_length & 0xFF;
  command[8] = block_length >> 8;

  return adcs_telecommand(
      command, 5);  //* tested + (command[6] * 256 * 256 * 256 + command[5] *
                    // 256 * 256 + command[4] * 256 + command[3])
}

/**
 * @brief
 * 		Advance File List Read Pointer
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_advance_file_list_read_pointer(void) {
  uint8_t command = ADVANCE_FILE_LIST_READ_POINTER_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Initiate File Upload
 * @param file_dest
 * 		Accepted parameters: EEPROM = 2, FLASH_PROGRAM_1 -
 * FLASH_PROGRAM_7 = 3-10, SD_USER_FILE_1 - SD_USER_FILE_8 = 11-17
 * @param block_size
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_initiate_file_upload(File_Upload_Dest file_dest,
                                           uint8_t block_size) {
  uint8_t command[3];
  command[0] = INITIATE_FILE_UPLOAD_ID;
  command[1] = file_dest;
  command[2] = block_size;
  return adcs_telecommand(command, 3);
}

/**
 * @brief
 * 		File Upload Packet
 * @param packet_number
 * 		Packet Number
 * @param file_bytes
 * 		File Bytes
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_file_upload_packet(uint16_t packet_number,
                                         char* file_bytes) {
  uint8_t command[23];
  command[0] = FILE_UPLOAD_PACKET_ID;
  command[1] = packet_number & 0xFF;
  command[2] = packet_number >> 8;
  memcpy(&command[3], file_bytes, 22);
  return adcs_telecommand(
      command,
      3);  //* Tested. returns ascii value of the string test: +command[6]
}

/**
 * @brief
 * 		Finalize Uploaded File Block
 * @param file_dest
 * 		File Destination
 * @param offset
 * 		Offset into file
 * @param block_length
 * 		Length of block
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_finalize_upload_block(File_Upload_Dest file_dest,
                                            uint32_t offset,
                                            uint16_t block_length) {
  uint8_t command[8];
  command[0] = FINALIZE_UPLOAD_BLOCK_ID;
  command[1] = file_dest;
  memcpy(&command[2], &offset, 4);
  command[6] = block_length & 0xFF;
  command[7] = block_length >> 8;
  return adcs_telecommand(command, 8);
}

/**
 * @brief
 * 		Reset HoleMap for Upload Block
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_reset_upload_block(void) {
  uint8_t command = RESET_UPLOAD_BLOCK_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Reset File List Read Pointer
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_reset_file_list_read_pointer(void) {
  uint8_t command = RESET_FILE_LIST_READ_POINTER_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Initiate Download Burst
 * @param msg_length
 * 		Message Length
 * @param ignore_hole_map
 * 		Ignore Hole Map
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_initiate_download_burst(uint8_t msg_length,
                                              bool ignore_hole_map) {
  uint8_t command[3];
  command[0] = INITIATE_DOWNLOAD_BURST_ID;
  command[1] = msg_length;
  command[2] = ignore_hole_map;
  return adcs_telecommand(command, 3);
}

/*************************** BootLoader TCs ***************************/
/**
 * @brief
 * 		Clear error flags
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_clear_err_flags(void) {
  uint8_t command = CLEAR_ERR_FLAGS_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Set boot index
 * @param index
 * 		one valid value: 1 = internal flash program
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_boot_index(uint8_t index) {
  uint8_t command[2];
  command[0] = SET_BOOT_INDEX_ID;
  if (index != 1) {
    return ADCS_INVALID_PARAMETERS;
  } else {
    command[1] = 1;
    return adcs_telecommand(command, 2);
  }
}

/**
 * @brief
 * 		Run the selected program
 * @attention
 * 		could be merged with the previous one in services
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_run_selected_program(void) {
  uint8_t command = RUN_SELECTED_PROGRAM_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Read the program information
 * @param index
 * 		Table 66 - Program index
 * 		0 : BootLoader, 1 : Internal flash program, 2 : EEPROM,
 * 		3-9: External flash program 1-7,
 * 		10-17: SD user file 1-8
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_read_program_info(uint8_t index) {
  uint8_t command[2];
  command[0] = READ_PROGRAM_INFO_ID;
  if (index < 0 || index > 18) {
    return ADCS_INVALID_PARAMETERS;
  } else {
    command[1] = index;
    return adcs_telecommand(command, 2);
  }
}

/**
 * @brief
 * 		Copy program to internal flash
 * @param index
 * 		Table 66 - Source Program index
 * 		0 : BootLoader, 1 : Internal flash program, 2 : EEPROM,
 * 3-9: External flash program 1-7, 10-17: SD user file 1-8
 * //* not good for services
 * @attention
 * 		flag = 0x5A overwrites the boot segment
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_copy_program_internal_flash(uint8_t index,
                                                  uint8_t overwrite_flag) {
  uint8_t command[3];
  command[0] = COPY_PROGRAM_INTERNAL_FLASH_ID;
  if (index < 0 || index > 18) {
    return ADCS_INVALID_PARAMETERS;
  } else {
    command[1] = index;
    command[2] = overwrite_flag;
    return adcs_telecommand(command, 3);
  }
}

/*************************** BootLoader TMs ***************************/

/**
 * @brief
 * 		Get BootLoader state
 * @param uptime
 * @param flags_arr
 * 		11 boolean flags from Table 69
 * 		sram1, sram2, sram_latch_not_recovered, sram_latch_recovered,
 * 		sd_initial_err, sd_read_err, sd_write_err, external_flash_err,
 * 		internal_flash_err, eeprom_err, bad_boot_reg, comms_radio_err
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_bootloader_state(uint16_t* uptime,
                                           uint8_t* flags_arr) {
  uint8_t telemetry[6];
  ADCS_returnState state;
  state = adcs_telemetry(GET_BOOTLOADER_STATE_ID, telemetry, 6);
  *uptime = (telemetry[1] << 8) + telemetry[0];
  uint32_t flags;
  memcpy(&flags, &telemetry[2], 4);
  for (int i = 0; i < 11; i++) {
    *(flags_arr + i) = (flags >> i) & 1;
  }
  return state;
}

/**
 * @brief
 * 		Get program information
 * @param index
 * 		Table 66 - Source Program index
 * @param busy
 * 		program busy reading
 * @param file_size
 * 		File size in bytes
 * @param crc16_checksum
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_program_info(uint8_t* index, bool* busy,
                                       uint32_t* file_size,
                                       uint16_t* crc16_checksum) {
  uint8_t telemetry[8];
  ADCS_returnState state;
  state = adcs_telemetry(GET_PROGRAM_INFO_ID, telemetry, 8);
  *index = telemetry[0];
  *busy = telemetry[1] & 1;
  *file_size = (telemetry[5] << 24) + (telemetry[4] << 16) +
               (telemetry[3] << 8) + telemetry[2];
  *crc16_checksum = (telemetry[7] << 8) + telemetry[6];
  return state;
}

/**
 * @brief
 * 		Progress of copy to internal flash operation
 * @param busy
 * 		busy copying
 * @param err
 * 		error in copying
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_copy_internal_flash_progress(bool* busy, bool* err) {
  uint8_t telemetry;
  ADCS_returnState state;
  state = adcs_telemetry(COPY_INTERNAL_FLASH_PROGRESS_ID, &telemetry, 1);
  *busy = telemetry & 1;
  *err = telemetry & 2;
  return state;
}

/*************************** ACP TCs ***************************/
/**
 * @brief
 * 		Deploy magnetometer boom (deployment actuation timeout value)
 * @param actuation_timeout
 * 		in seconds
 * @attention
 * 		uses a TC_ID that has been used before!
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_deploy_magnetometer_boom(uint8_t actuation_timeout) {
  uint8_t command[2];
  command[0] = DEPLOY_MAGNETOMETER_BOOM_ID;
  command[1] = actuation_timeout;
  return adcs_telecommand(command, 2);
}

/**
 * @brief
 * 		Set enabled state and control loop behavior
 * @param state : ADCS loop state
 * 		0 : inactive
 * 		1 : 1Hz loop active
 * 		2 : execute when triggered
 * 		3 : in simulation mode
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_enabled_state(uint8_t state) {
  uint8_t command[2];
  command[0] = ADCS_RUN_MODE_ID;
  command[1] = state;
  return adcs_telecommand(command, 2);
}

/**
 * @brief
 * 		Clear latched error flags
 * @param adcs_flag: ADCS error flags
 * @param hk_flag: Housekeeping error flags
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_clear_latched_errs(bool adcs_flag, bool hk_flag) {
  uint8_t command[2];
  command[0] = CLEAR_LATCHED_ERRS_ID;
  command[1] = adcs_flag + 2 * hk_flag;
  return adcs_telecommand(
      command,
      2);  //* add "+ command[1]" for test and let it fail and check the value
}

/**
 * @brief
 * 		Set attitude control mode
 * @param ctrl_mode: 0-15: Refer to Table 78 in the manual
 * @param timeout: control timeout duration in seconds (0XFFFF: infinity)
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_attitude_ctrl_mode(uint8_t ctrl_mode,
                                             uint16_t timeout) {
  uint8_t command[4];
  command[0] = SET_ATT_CONTROL_MODE_ID;
  command[1] = ctrl_mode;
  command[2] = timeout & 0xFF;
  command[3] = timeout >> 8;
  return adcs_telecommand(command, 4);  //*  + (256*command[3]+command[2])
}

/**
 * @brief
 * 		Set attitude estimation mode
 * @param mode: 0-7 : Refer to Table 80 in the manual
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_attitude_estimate_mode(uint8_t mode) {
  uint8_t command[2];
  command[0] = SET_ATT_ESTIMATE_MODE_ID;
  command[1] = mode;
  return adcs_telecommand(command, 2);
}

/**
 * @brief
 * 		Trigger ADCS loop
 * 	@attention
 * 		ADCS_set_enabled_state(2) must have been called in order for
 * this to operate
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_trigger_adcs_loop(void) {
  uint8_t command = TRIGGER_ADCS_LOOP_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Trigger ADCS loop with simulated sensor data
 * 	@parameter sim_data
 * 		127 bytes of data. Refer to table 84
 * 	@attention
 * 		ADCS_set_enabled_state(2) must have been called in order for
 * this to operate
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_trigger_adcs_loop_sim(
    uint8_t* sim_data) {  //* Doesn't make sense to implement by individual
                          // parameters.
  uint8_t command[128];
  command[0] = TRIGGER_ADCS_LOOP_SIM_ID;
  memcpy(&command[1], sim_data, 127);
  return adcs_telecommand(command, 128);
}

/**
 * @brief
 * 		Set ASGP4 run mode
 * @param mode (Table 87)
 * 		0 : off
 * 		1 : waiting for trigger
 * 		2 : background
 * 		3 : augment
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_ASGP4_rune_mode(uint8_t mode) {
  uint8_t command[2];
  command[0] = ASGP4_RUN_MODE_ID;
  command[1] = mode;
  return adcs_telecommand(command, 2);
}

/**
 * @brief
 * 		Trigger a start of the ASGP4 process
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_trigger_ASGP4(void) {
  uint8_t command = ASGP4_TRIGGER_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Set magnetometer operation mode and which is used
 * @param mode (Table 90)
 * 		0 : Main MTM through signal
 * 		1 : Redundant MTM through signal
 * 		2 : Main MTM through motor
 * 		3 : None
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_MTM_op_mode(uint8_t mode) {
  uint8_t command[2];
  command[0] = SET_MTM_OP_MODE_ID;
  command[1] = mode;
  return adcs_telecommand(command, 2);
}

/**
 * @brief
 * 		Convert raw or bmp files to JPG file
 * @param source
 * 		source file counter
 * @param QF
 * 		quality factor
 * @param white_balance
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_cnv2jpg(uint8_t source, uint8_t QF,
                              uint8_t white_balance) {
  uint8_t command[4];
  command[0] = CNV2JPG_ID;
  command[1] = source;
  command[1] = QF;
  command[1] = white_balance;
  return adcs_telecommand(command, 4);
}

/**
 * @brief
 * 		Save image from one of cameras to SD
 * @param camera
 * 		Camera selection:
 * 		0 : CubeSense Cam1
 * 		1 : CubeSense Cam2
 * 		2 : CubeStar
 * @param img_size
 * 		0 : 1024 * 1024
 * 		1 : 512*512
 * 		2 : 256*256
 * 		3 : 128*128
 * 		4 : 64*64
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_save_img(uint8_t camera, uint8_t img_size) {
  uint8_t command[3];
  command[0] = SAVE_IMG_ID;
  command[1] = camera;
  command[1] = img_size;
  return adcs_telecommand(command, 3);
}

/**
 * @brief
 * 		Set magnetorquer output
 * @attention
 * 		ADCS_set_attitude_ctrl_mode(0, ) must be called. (Control mode
 * None)
 * @attention
 * 		If using the raw value, perform /1000 and note that the value
 * must be <2^15
 * @param x,y,z
 * 		Commanded x,y,z-torquer duty cycle
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_magnetorquer_output(int16_t x, int16_t y, int16_t z) {
  uint8_t command[7];
  command[0] = SET_MAGNETORQUER_OUTPUT_ID;
  command[1] = x & 0xFF;
  command[2] = x >> 8;
  command[3] = y & 0xFF;
  command[4] = y >> 8;
  command[5] = z & 0xFF;
  command[6] = z >> 8;
  return adcs_telecommand(command, 7);  //* + (256*command[6] + command[5])
}

/**
 * @brief
 * 		Set wheel speed
 * @attention
 * 		ADCS_set_attitude_ctrl_mode(0, ) must be called. (Control mode
 * None)
 * @param x,y,z
 * 		Commanded x,y,z-wheel speed in rpm
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_wheel_speed(int16_t x, int16_t y, int16_t z) {
  uint8_t command[7];
  command[0] = SET_WHEEL_SPEED_ID;
  command[1] = x & 0xFF;
  command[2] = x >> 8;
  command[3] = y & 0xFF;
  command[4] = y >> 8;
  command[5] = z & 0xFF;
  command[6] = z >> 8;
  return adcs_telecommand(command, 7);
}

/**
 * @brief
 * 		Save current configuration to flash memory
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_save_config(void) {
  uint8_t command = SAVE_CONFIG_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Save current orbit parameters to flash memory
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_save_orbit_params(void) {
  uint8_t command = SAVE_ORBIT_PARAMS;
  return adcs_telecommand(&command, 1);
}

/*************************** ACP TMs ***************************/
/************************* ADCS State **************************/
/**
 * @brief
 * 		Get ADCS current state
 * @param data
 * 		A struct of floats defined in adcs_handler.h
 * 		Refer to table 149
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_current_state(adcs_state* data) {
  uint8_t telemetry[54];
  ADCS_returnState state;
  state = adcs_telemetry(ADCS_STATE, telemetry, 54);
  data->att_estimate_mode = telemetry[0] & 0xF;     // Refer to table 80
  data->att_ctrl_mode = (telemetry[0] >> 4) & 0xF;  // Refer to table 78
  data->run_mode = telemetry[1] & 0x3;              // Refer to table 75
  data->ASGP4_mode = (telemetry[1] >> 2) & 0x3;     // Refer to table 87
  for (int i = 0; i < 4; i++) {
    *(data->flags_arr + i) = (telemetry[1] >> (i + 4)) & 1;
  }
  uint32_t flags1;
  memcpy(&flags1, &telemetry[2], 4);
  for (int i = 0; i < 32; i++) {
    *(data->flags_arr + i + 4) = (flags1 >> i) & 1;
  }
  uint32_t flags2;
  memcpy(&flags2, &telemetry[6], 4);
  for (int i = 0; i < 3; i++) {
    *(data->flags_arr + i + 36) = (flags2 >> i) & 1;
  }
  // position:52
  data->MTM_sample_mode = (flags2 >> 4) & 0x3;  // Refer to table 90
  for (int i = 6; i < 18; i++) {
    *(data->flags_arr + i + 34) =
        (flags2 >> i) & 1;  // 34 because 2 were for sample mode
  }
  get_xyz(&data->est_angle, &telemetry[12], 0.01);  // [deg]
  get_xyz16(&data->est_quaternion, &telemetry[18]);
  get_xyz(&data->est_angular_rate, &telemetry[24], 0.01);  // [deg/s]
  get_xyz(&data->ECI_pos, &telemetry[30], 0.25);           // [km]
  get_xyz(&data->ECI_vel, &telemetry[36], 0.25);           // [m/s]
  get_xyz(&data->longlatalt, &telemetry[42], 0.01);        // [deg, deg, km]
  if (telemetry[47] >> 7) {  // since it is uint16 and has been treated as int16
    data->longlatalt.z = 0.01 * (telemetry[47] << 8 | telemetry[46]);
  }
  get_xyz16(&data->ecef_pos, &telemetry[48]);  // [m]
  return state;
}

/************************* General **************************/
/**
 * @brief
 * 		JPG conversion progress
 * @param percentage
 * 		progress in percentage
 * @param result
 * 		conversion result:
 * 		0 : nothing converted
 * 		1 : success
 * 		2 : file load error
 * 		3 : busy
 * @param file_counter
 * 		Output file counter
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_jpg_cnv_progress(uint8_t* percentage, uint8_t* result,
                                           uint8_t* file_counter) {
  uint8_t telemetry[3];
  ADCS_returnState state;
  state = adcs_telemetry(JPG_CNV_PROGRESS_ID, telemetry, 3);
  *percentage = telemetry[0];
  *result = telemetry[1];
  *file_counter = telemetry[2];
  return state;
}

/**
 * @brief
 * 		Get flags regarding CubeACP state
 * @param flags_arr
 * 		6 boolean flags from Table 101
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_cubeACP_state(uint8_t* flags_arr) {
  uint8_t telemetry;
  ADCS_returnState state;
  state = adcs_telemetry(CUBEACP_STATE_FLAGS_ID, &telemetry, 1);
  for (int i = 0; i < 6; i++) {
    *(flags_arr + i) = (telemetry >> i) & 1;
  }
  return state;
}

/**
 * @brief
 * 		Get execution times of ACP functions
 * @param adcs_update
 * 		time to perform complete adcs update [ms]
 * @param sensor_comms
 * 		time to perform sensor/actuator communications [ms]
 * @param sgp4_propag
 * 		time to perform SGP4 propagator [ms]
 * @param igrf_model
 * 		time to execute IGRF computation [ms]
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_execution_times(uint16_t* adcs_update,
                                          uint16_t* sensor_comms,
                                          uint16_t* sgp4_propag,
                                          uint16_t* igrf_model) {
  uint8_t telemetry[8];
  ADCS_returnState state;
  state = adcs_telemetry(ADCS_EXE_TIMES_ID, telemetry, 8);
  *adcs_update = (telemetry[1] << 8) + telemetry[0];
  *sensor_comms = (telemetry[3] << 8) + telemetry[2];
  *sgp4_propag = (telemetry[5] << 8) + telemetry[4];
  *igrf_model = (telemetry[7] << 8) + telemetry[6];
  return state;
}

/**
 * @brief
 * 		Returns information about the ACP loop
 * @param time
 * 		 Time since the start of the current loop iteration [ms]
 * @param execution_point
 * 		Indicates which part of the loop is currently executing
 * 		12 possible values. Refer to Table 168
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_ACP_loop_stat(uint16_t* time,
                                        uint8_t* execution_point) {
  uint8_t telemetry[3];
  ADCS_returnState state;
  state = adcs_telemetry(ACP_EXE_STATE_ID, telemetry, 3);
  *time = (telemetry[1] << 8) + telemetry[0];
  *execution_point = telemetry[2];
  return state;
}

/**
 * @brief
 * 		Status of Image Capture and Save Operation
 * @param percentage
 * 		progress in percentage
 * @param status
 * 		conversion result:
 * 		0 : no error
 * 		1 : timeout waiting for sensor
 * 		2 : timeout waiting for frame
 * 		3 : checksum mismatch between downloaded and unit frame
 * 		4 : error writing to SD
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_img_save_progress(uint8_t* percentage,
                                            uint8_t* status) {
  uint8_t telemetry[2];
  ADCS_returnState state;
  state = adcs_telemetry(IMG_CAPTURE_SAVE_OP_STAT, telemetry, 2);
  *percentage = telemetry[0];
  *status = telemetry[1];
  return state;
}

/*********************** ADCS Measurement ************************/

/**
 * @brief
 * 		Get Calibrated sensor measurements
 * @param measurements
 * 		A struct of floats defined in adcs_handler.h
 * 		Refer to table 150
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_measurements(adcs_measures* measurements) {
  uint8_t telemetry[72];
  ADCS_returnState state;
  state = adcs_telemetry(ADCS_MEASUREMENTS_ID, telemetry, 72);
  get_xyz(&measurements->magnetic_field, &telemetry[0], 0.01);  // [uT]
  get_xyz(&measurements->coarse_sun, &telemetry[6], 0.0001);
  get_xyz(&measurements->sun, &telemetry[12], 0.0001);
  get_xyz(&measurements->nadir, &telemetry[18], 0.0001);
  get_xyz(&measurements->angular_rate, &telemetry[24], 0.01);  // [deg/s]
  get_xyz(&measurements->wheel_speed, &telemetry[30], 1);      // [rpm]
  get_xyz(&measurements->star1b, &telemetry[36], 0.0001);
  get_xyz(&measurements->star1o, &telemetry[42], 0.0001);
  get_xyz(&measurements->star2b, &telemetry[48], 0.0001);
  get_xyz(&measurements->star2o, &telemetry[54], 0.0001);
  get_xyz(&measurements->star3b, &telemetry[60], 0.0001);
  get_xyz(&measurements->star3o, &telemetry[66], 0.0001);
  return state;
}

/*********************** ADCS Actuator ************************/
/**
 * @brief
 * 		Get actuator commands
 * @param measurements
 * 		A struct of floats defined in adcs_handler.h
 * 		Refer to table 151
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_actuator(adcs_actuator* commands) {
  uint8_t telemetry[12];
  ADCS_returnState state;
  state = adcs_telemetry(ACTUATOR_ID, telemetry, 12);
  get_xyz(&commands->magnetorquer, &telemetry[0], 100);  // [s]
  get_xyz(&commands->wheel_speed, &telemetry[6], 1);     // [rpm]
  return state;
}

/*********************** ADCS Estimation ************************/
/**
 * @brief
 * 		Get the esimation meta-data
 * @param data
 * 		A struct of floats defined in adcs_handler.h
 * 		Refer to table 152
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_estimation(adcs_estimate* data) {
  uint8_t telemetry[42];
  ADCS_returnState state;
  state = adcs_telemetry(ESTIMATION_ID, telemetry, 12);
  get_xyz(&data->igrf_magnetic_field, &telemetry[0], 0.01);  // [uT]
  get_xyz(&data->sun, &telemetry[6], 0.0001);
  get_xyz(&data->gyro_bias, &telemetry[12], 0.001);  // [deg/s]
  get_xyz(&data->innovation, &telemetry[18], 0.0001);
  get_xyz(&data->quaternion_err, &telemetry[24], 0.0001);
  get_xyz(&data->quaternion_covar, &telemetry[30], 0.001);
  get_xyz(&data->angular_rate_covar, &telemetry[36], 0.001);
  return state;
}

/**
 * @brief
 * 		Get ASGP4 TLEs
 * @param complete
 * 		ASGP4 process complete
 * @param err
 * 		The error state that ASGP4 module is in (Table 171)
 * 		0 : no error
 * 		1 : timeout
 * 		2 : position error exceeding Th
 * 		3 : overflow
 * @param asgp4
 * 		A struct of floats defined in adcs_handler.h
 * 		Refer to table 170
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_ASGP4(bool* complete, uint8_t* err,
                                adcs_asgp4* asgp4) {
  uint8_t telemetry[33];
  ADCS_returnState state;
  state = adcs_telemetry(ASGP4_TLEs_ID, telemetry, 2);
  *complete = telemetry[0] & 1;
  *err = telemetry[0] >> 1;
  memcpy(&asgp4->epoch, &telemetry[1], 4);
  memcpy(&asgp4->inclination, &telemetry[5], 4);
  memcpy(&asgp4->RAAN, &telemetry[9], 4);
  memcpy(&asgp4->ECC, &telemetry[13], 4);
  memcpy(&asgp4->AOP, &telemetry[17], 4);
  memcpy(&asgp4->MA, &telemetry[21], 4);
  memcpy(&asgp4->MM, &telemetry[25], 4);
  memcpy(&asgp4->Bstar, &telemetry[29], 4);
  return state;
}

/********************* ADCS Raw Sensor Measurements **********************/
/**
 * @brief
 * 		Camera sensor capture and detection result
 * @param centroid_x
 * 		azimuth angle
 * @param centroid_y
 * 		elevation angle
 * @param capture_stat
 * 		Camera capture status: 0-5 Refer to table 123
 * @param detect_result
 * 		Camera detection result: 0-7 Refer to table 124
 * @param address
 * 		the position in the telemetry frame where the data is located
 */
void get_cam_sensor(cam_sensor* cam, uint8_t* address) {
  cam->centroid_x = uint82int16(*address, *(address + 1));
  cam->centroid_y = uint82int16(*(address + 2), *(address + 3));
  cam->capture_stat = *(address + 4);   //* check
  cam->detect_result = *(address + 5);  //* check
}

/**
 * @brief
 * 		Get raw sensor measurements
 * @param measurements
 * 		A struct of floats defined in adcs_handler.h
 * 		Refer to table 153
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_raw_sensor(adcs_raw_sensor* measurements) {
  uint8_t telemetry[34];
  ADCS_returnState state;
  state = adcs_telemetry(RAW_SENSOR_MEASUREMENTS_ID, telemetry, 34);
  get_cam_sensor(&measurements->cam1, &telemetry[0]);
  get_cam_sensor(&measurements->cam2, &telemetry[6]);
  for (int i = 0; i < 10; i++) {
    *(measurements->css + i) = telemetry[i + 12];  //* check
  }
  get_xyz16(&measurements->MTM, &telemetry[22]);
  get_xyz16(&measurements->rate, &telemetry[28]);
  return state;
}

/**
 * @brief
 * 		Camera sensor capture and detection result
 * @param pos
 * 		ECEF position [m]
 * @param vel
 * 		ECEF velocity [m/s]
 * @param address
 * 		the position in the telemetry frame where the data is located
 */
void get_ecef(ecef* coordinate, uint8_t* address) {
  coordinate->pos = uint82int32(address);  //* check
  coordinate->vel = uint82int16(*(address + 4), *(address + 5));
}

/**
 * @brief
 * 		Get raw GPS measurements
 * @param measurements
 * 		A struct of floats defined in adcs_handler.h
 * 		Refer to table 158
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_raw_GPS(adcs_raw_gps* measurements) {
  uint8_t telemetry[36];
  ADCS_returnState state;
  state = adcs_telemetry(RAW_GPS_MEASUREMENTS_ID, telemetry, 36);
  measurements->sol_stat = telemetry[0];
  measurements->tracked_sats = telemetry[1];
  measurements->usedInSol_sats = telemetry[2];
  measurements->xyz_lof_count = telemetry[3];
  measurements->range_log_count = telemetry[4];
  measurements->response_msg = telemetry[5];
  measurements->reference_week = telemetry[7] << 8 | telemetry[6];
  measurements->time = telemetry[11] << 24 | telemetry[10] << 16 |
                       telemetry[9] << 8 | telemetry[8];
  get_ecef(&measurements->x, &telemetry[12]);
  get_ecef(&measurements->y, &telemetry[18]);
  get_ecef(&measurements->z, &telemetry[24]);
  measurements->pos_std_dev.x = telemetry[30] * 0.1;  // check -> or .
  measurements->pos_std_dev.y = telemetry[31] * 0.1;
  measurements->pos_std_dev.z = telemetry[32] * 0.1;
  measurements->vel_std_dev.x = telemetry[33];
  measurements->vel_std_dev.y = telemetry[34];
  measurements->vel_std_dev.z = telemetry[35];
  // get_xyzu8(&measurements->vel_std_dev, &telemetry[33]); // maybe have it
  // implicit
  return state;
}

/**
 * @brief
 * 		Get raw data for stars
 * @param address
 * 		the position in the telemetry frame where the data is located
 * @param i
 * 		star index
 */
void get_star_data(star_data* coordinate, uint8_t* address, uint8_t i) {
  coordinate->confidence = *(address + i);  // percent
  coordinate->magnitude = *(address + 4 + 2 * i) << 8 | *(address + 3 + 2 * i);
  coordinate->catalouge_num =
      *(address + 10 + 6 * i) << 8 | *(address + 9 + 6 * i);
  if ((*(address + 12 + 6 * i)) >> 7) {
    coordinate->centroid_x =
        -1 * (~(*(address + 12 + 6 * i) << 8 | *(address + 11 + 6 * i)) + 1);
  } else {
    coordinate->centroid_x =
        *(address + 12 + 6 * i) << 8 | *(address + 11 + 6 * i);
  }
  if ((*(address + 14 + 6 * i)) >> 7) {
    coordinate->centroid_y =
        -1 * (~(*(address + 14 + 6 * i) << 8 | *(address + 13 + 6 * i)) + 1);
  } else {
    coordinate->centroid_y =
        *(address + 14 + 6 * i) << 8 | *(address + 13 + 6 * i);
  }
}

/**
 * @brief
 * 		Get raw star tracker measurements
 * @param measurements
 * 		A struct of floats defined in adcs_handler.h
 * 		Refer to table 159
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_star_tracker(adcs_star_track* measurements) {
  uint8_t telemetry[54];
  ADCS_returnState state;
  state = adcs_telemetry(RAW_STAR_TRACKER_ID, telemetry, 54);
  measurements->detected_stars = telemetry[0];
  measurements->img_noise = telemetry[1];
  measurements->invalid_stars = telemetry[2];
  measurements->identified_stars = telemetry[3];
  measurements->identification_mode = telemetry[4];
  measurements->img_dark_val = telemetry[5];
  for (int i = 0; i < 8; i++) {
    *(measurements->flags_arr + i) = (telemetry[6] >> i) & 1;
  }
  measurements->sample_T = telemetry[8] << 8 | telemetry[7];
  get_star_data(&measurements->star1, &telemetry[9], 0);
  get_star_data(&measurements->star2, &telemetry[9], 1);
  get_star_data(&measurements->star3, &telemetry[9], 2);
  measurements->capture_t = telemetry[37] << 8 | telemetry[36];         // [ms]
  measurements->detect_t = telemetry[39] << 8 | telemetry[38];          // [ms]
  measurements->identification_t = telemetry[41] << 8 | telemetry[40];  // [ms]
  get_xyz(&measurements->estimated_rate, &telemetry[42], 0.0001);
  get_xyz(&measurements->estimated_att, &telemetry[48], 0.0001);
  return state;
}

/**
 * @brief
 * 		Get secondary Magnetometer raw measurements
 * @param Mag
 * 		2nd MTM sampled A/D value
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_MTM2_measurements(xyz16* Mag) {
  uint8_t telemetry[6];
  ADCS_returnState state;
  state = adcs_telemetry(MTM2_MEASUREMENTS_ID, telemetry, 6);
  get_xyz16(Mag, &telemetry[0]);
  return state;
}

/******************* ADCS Power & Temperature ********************/
/**
 * @brief
 * 		a supplementary function for the ADCS power measurements
 * @param measurement
 * 		the measured parameter (Table 154)
 * @param address
 * 		the position in the telemetry frame where the data is located
 * @param coef
 * 		formatted value = rawval * coef;
 */
void get_current(float* measurement, uint8_t* address, float coef) {  //*check
  *measurement = coef * uint82uint16(*address, *(address + 1));
}

/**
 * @brief
 * 		a supplementary function for the ADCS temperature measurements
 * @param measurement
 * 		the measured parameter (Table 154)
 * @param address
 * 		the position in the telemetry frame where the data is located
 * @param coef
 * 		formatted value = rawval * coef;
 */
void get_temp(float* measurement, uint8_t* address, float coef) {
  *measurement = coef * uint82int16(*address, *(address + 1));
}

/**
 * @brief
 * 		Get Power & Temperature measurements
 * @param measurements
 * 		A struct of floats defined in adcs_handler.h
 * 		Refer to table 154
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_power_temp(adcs_pwr_temp* measurements) {
  uint8_t telemetry[38];
  ADCS_returnState state;
  state = adcs_telemetry(POWER_TEMP_ID, telemetry, 38);

  get_current(&measurements->cubesense1_3v3_I, &telemetry[0], 0.1);      // [mA]
  get_current(&measurements->cubesense1_camSram_I, &telemetry[2], 0.1);  // [mA]
  get_current(&measurements->cubesense2_3v3_I, &telemetry[4], 0.1);      // [mA]
  get_current(&measurements->cubesense2_camSram_I, &telemetry[6], 0.1);  // [mA]
  get_current(&measurements->cubecontrol_3v3_I, &telemetry[8],
              0.48828125);  // [mA]
  get_current(&measurements->cubecontrol_5v_I, &telemetry[10],
              0.48828125);  // [mA]
  get_current(&measurements->cubecontrol_vBat_I, &telemetry[12],
              0.48828125);                                          // [mA]
  get_current(&measurements->wheel1_I, &telemetry[14], 0.01);       // [mA]
  get_current(&measurements->wheel2_I, &telemetry[16], 0.01);       // [mA]
  get_current(&measurements->wheel3_I, &telemetry[18], 0.01);       // [mA]
  get_current(&measurements->cubestar_I, &telemetry[20], 0.01);     // [mA]
  get_current(&measurements->magnetorquer_I, &telemetry[20], 0.1);  // [mA]

  get_temp(&measurements->cubestar_temp, &telemetry[22], 0.01);  // [C]
  get_temp(&measurements->MCU_temp, &telemetry[24], 1);          // [C]
  get_temp(&measurements->MTM_temp, &telemetry[26], 0.1);        // [C]
  get_temp(&measurements->MTM2_temp, &telemetry[28], 0.1);       // [C]
  get_xyz16(&measurements->rate_sensor_temp, &telemetry[30]);    // [C]

  return state;
}

/************************* ACP Config Msgs *************************/
/***************************** General *****************************/
/**
 * @brief
 * 		Control the power state of some components (Table 184)
 * @param control
 * 		an array with the values defined in Table 185:
 * 		0 : off
 * 		1 : on
 * 		2 : keep the same
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_power_control(uint8_t* control) {
  uint8_t command[4];
  command[0] = SET_POWER_CONTROL_ID;
  for (int i = 0; i < 4; i++) {
    command[1] = command[1] | (*(control + i) << 2 * i);
  }
  for (int i = 0; i < 4; i++) {
    command[2] = command[2] | (*(control + 4 + i) << 2 * i);
  }
  for (int i = 0; i < 2; i++) {
    command[3] = command[3] | (*(control + i) << 2 * i);
  }
  return adcs_telecommand(command, 4);
}

/**
 * @brief
 * 		Get the power state of some components (Table 184)
 * @param control
 * 		an array with the values defined in Table 185:
 * 		0 : off
 * 		1 : on
 * 		2 : keep the same
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_power_control(uint8_t* control) {
  uint8_t telemetry[3];
  ADCS_returnState state;
  state = adcs_telemetry(GET_POWER_CONTROL_ID, telemetry, 3);

  for (int i = 0; i < 4; i++) {
    *(control + i) = (telemetry[0] >> 2 * i) & 0x3;
  }
  for (int i = 0; i < 4; i++) {
    *(control + i + 4) = (telemetry[1] >> 2 * i) & 0x3;
  }
  for (int i = 0; i < 2; i++) {
    *(control + i) = (telemetry[2] >> 2 * i) & 0x3;
  }
  return state;
}

/**
 * @brief
 * 		Set commanded attitude angles (Table 186)
 * @param att_angle
 * 		roll, pitch, yaw angle
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_attitude_angle(xyz att_angle) {
  uint8_t command[7];
  command[0] = SET_ATT_ANGLE_ID;
  float coef = 0.01;
  xyz16 raw_val;
  raw_val.x = att_angle.x / coef;
  raw_val.y = att_angle.y / coef;
  raw_val.z = att_angle.z / coef;
  memcpy(&command[1], &raw_val, 6);
  return adcs_telecommand(command, 7);
}

/**
 * @brief
 * 		Get commanded attitude angles (Table 186)
 * @param att_angle
 * 		roll, pitch, yaw angle
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_attitude_angle(xyz* att_angle) {
  uint8_t telemetry[6];
  ADCS_returnState state;
  state = adcs_telemetry(GET_ATT_ANGLE_ID, telemetry, 6);
  float coef = 0.01;
  get_xyz(att_angle, &telemetry[0], coef);
  return state;
}

/**
 * @brief
 * 		Set target reference for tracking control mode (Table 187)
 * @param att_angle
 * 		longitude, latitude, angle
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_track_controller(xyz target) {
  uint8_t command[13];
  command[0] = SET_TRACK_CTRLER_TARGET_REF_ID;
  memcpy(&command[1], &target, 6);
  return adcs_telecommand(command, 13);
}

/**
 * @brief
 * 		Get target reference for tracking control mode (Table 187)
 * @param att_angle
 * 		longitude, latitude, angle
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_track_controller(xyz* target) {
  uint8_t telemetry[12];
  ADCS_returnState state;
  state = adcs_telemetry(GET_TRACK_CTRLER_TARGET_REF_ID, telemetry, 12);
  memcpy(&target->x, &telemetry[0], 4);
  memcpy(&target->y, &telemetry[4], 4);
  memcpy(&target->z, &telemetry[8], 4);
  return state;
}

/**
 * @brief
 * 		Log selection and period for LOG 1,2
 * @param flags_arr
 * 		Up to 80 flags indicating which telemetry frames should be
 * logged
 * @param period
 * 		log period (0 for stop)
 * @param dest
 * 		log destination (Table 211)
 * 		0 : primary SD card
 * 		1 : secondary SD card
 * @param log
 * 		1 : log1
 * 		2 : log2
 * 		3 : UART
 * @attention
 * 		dest for UART is not needed.
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_log_config(uint8_t* flags_arr, uint16_t period,
                                     uint8_t dest, uint8_t log) {
  uint8_t command[14];
  command[0] = SET_SD_LOG1_CONFIG_ID + (log - 1);
  for (int j = 0; j < 10; j++) {
    for (int i = 0; i < 8; i++) {
      command[j + 1] = command[j + 1] | (*(flags_arr + (8 * j) + i) << i);
    }
  }
  memcpy(&command[11], &period, 2);
  command[13] = dest;
  ADCS_returnState state;
  if (log == 3) {
    state = adcs_telecommand(command, 13);
  } else {
    state = adcs_telecommand(command, 14);
  }
  return state;
}

/**
 * @brief
 * 		Get log selection and period for LOG 1,2
 * @param flags_arr
 * 		Up to 80 flags indicating which telemetry frames should be
 * logged
 * @param period
 * 		log period (0 for stop)
 * @param dest
 * 		log destination (Table 211)
 * 		0 : primary SD card
 * 		1 : secondary SD card
 * @param log
 * 		1 : log1
 * 		2 : log2
 * 		3 : dest
 * @attention
 * 		dest for UART is not used.
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_log_config(uint8_t* flags_arr, uint16_t* period,
                                     uint8_t* dest, uint8_t log) {
  uint8_t telemetry[13];
  ADCS_returnState state;
  uint8_t TM_ID = GET_SD_LOG1_CONFIG_ID + (log - 1);
  if (TM_ID == GET_UART_LOG_CONFIG_ID) {
    state = adcs_telemetry(TM_ID, telemetry, 12);
  } else {
    state = adcs_telemetry(TM_ID, telemetry, 13);
  }

  for (int j = 0; j < 10; j++) {
    for (int i = 0; i < 8; i++) {
      *(flags_arr + (8 * j) + i) = (telemetry[j] >> i) & 1;
    }
  }
  *period = telemetry[11] << 8 | telemetry[10];
  if (TM_ID == GET_UART_LOG_CONFIG_ID) {
    *dest = 2;
  } else {
    *dest = telemetry[12];
  }
  return state;
}

/**
 * @brief
 * 		Set reference unit vector for inertial pointing control mode
 * (Table 214)
 * @param inter_ref
 * 		Intertial reference
 * @param coef
 * 		formatted value = rawval * coef;
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_set_intertial_ref(xyz inter_ref) {
  uint8_t command[7];
  command[0] = SET_INTERTIAL_POINT_ID;
  float coef = 0.0001;
  xyz16 raw_val;
  raw_val.x = inter_ref.x / coef;
  raw_val.y = inter_ref.y / coef;
  raw_val.z = inter_ref.z / coef;
  memcpy(&command[1], &raw_val, 6);
  return adcs_telecommand(command, 7);
}

/**
 * @brief
 * 		Get reference unit vector for inertial pointing control mode
 * (Table 214)
 * @param inter_ref
 * 		Intertial reference
 * @param coef
 * 		formatted value = rawval * coef;
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_get_intertial_ref(xyz* inter_ref) {
  uint8_t telemetry[6];
  ADCS_returnState state;
  state = adcs_telemetry(GET_INTERTIAL_POINT_ID, telemetry, 6);
  float coef = 0.0001;
  get_xyz(inter_ref, &telemetry[0], coef);
  // get_xyz(&data->angular_rate_covar, &telemetry[36], 0.001);
  return state;
}

/************************* Configuration *************************/
