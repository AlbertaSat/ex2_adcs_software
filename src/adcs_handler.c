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
 *	 	 	 	 	 	 	 FILE_TYPE_JPG_IMAGE =
 *3, FILE_TYPE_BMP_IMAGE = 4, FILE_TYPE_INDEX = 15
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
 *	 	 	 	 	 	 	 FILE_TYPE_JPG_IMAGE =
 *3, FILE_TYPE_BMP_IMAGE = 4, FILE_TYPE_INDEX = 15
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
 * 		Accepted parameters: EEPROM = 2,
 * 							 FLASH_PROGRAM_1 - FLASH_PROGRAM_7
 * = 3-10, SD_USER_FILE_1 - SD_USER_FILE_8 = 11-17
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
 * @param 11 boolean flags from Table 69
 * 		sram1, sram2, sram_latch_not_recovered, sram_latch_recovered,
 * sd_initial_err, sd_read_err, sd_write_err, external_flash_err,
 * internal_flash_err, eeprom_err, bad_boot_reg, comms_radio_err
 * @attention
 * 		May be replaced with an array of flags
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
