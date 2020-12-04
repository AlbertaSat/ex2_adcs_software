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

#include "adcs_handler.h"

#include <string.h>

#include "adcs_io.h"
#include "adcs_types.h"

#define USE_UART
//#define USE_I2C

/**
 * @brief
 *
 * @details
 * 		<more detailed description of the function>
 * @return
 * 		<if applicable, brief description of what is returned by the
 * function>
 */
ADCS_returnState adcs_telecommand(uint8_t* command, uint32_t length) {
  // send and receive data via selected data protocol (i2c, SPI, UART)

  // //Send SOM bytes
  // uart_send(1, &ESC_CHAR);
  // uart_send(1, &SOM);

  // //Send Command
  // uart_send(length, command);

  // //Send EOM
  // uart_send(1, &ESC_CHAR);
  // uart_send(1, &EOM);

  // get Acknowledgement
  ADCS_returnState ack = ADCS_OK;

#ifdef USE_UART
  ack = send_uart_telecommand(command, length);
#elif USE_I2C
  ack = send_i2c_telecommand(command, length);
#endif

  return ack;
}

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

///**
// * @brief
// * 		Perform a reset.
// * @details
// * 		<more detailed description of the function>
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_reset() {
//  uint32_t command[2];
//  command[0] = RESET_ID;
//  command[1] = ADCS_MAGIC_NUMBER; // Magic number
//  return adcs_telecommand(command, 2);
//}
//
///**
// * @brief
// * 		Reset pointer to log buffer (from where LastLogEvent TLM is
// returned)
// * @details
// * 		<more detailed description of the function>
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_reset_log_pointer() {
//  uint32_t command = RESET_LOG_POINTER_ID;
//  return adcs_telecommand(&command, 1);
//}
//
///**
// * @brief
// * 		Advance pointer to log buffer (from where LastLogEvent TLM is
// returned)
// * @details
// * 		<more detailed description of the function>
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_advance_log_pointer() {
//  uint32_t command = ADVANCE_LOG_POINTER_ID;
//  return adcs_telecommand(&command, 1);
//}
//
///**
// * @brief
// * 		Reset Boot Registers
// * @details
// * 		<more detailed description of the function>
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_reset_boot_registers() {
//  uint32_t command = RESET_BOOT_REGISTERS_ID;
//  return adcs_telecommand(&command, 1);
//}
//
///**
// * @brief
// * 		Format SD Card
// * @details
// * 		<more detailed description of the function>
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_format_sd_card() {
//  uint32_t command[2];
//  command[0] = FORMAT_SD_CARD_ID;
//  command[1] = ADCS_MAGIC_NUMBER; // magic number
//  return adcs_telecommand(command, 2);
//}
//
///**
// * @brief
// * 		Erase file
// * @details
// * 		<more detailed description of the function>
// * @param file_type
// * 		Accepted parameters: FILE_TYPE_TELEMETERY_LOG,
// FILE_TYPE_JPG_IMAGE,
// *                          FILE_TYPE_BMP_IMAGE or FILE_TYPE_INDEX.
// * @param file_counter
// * 		<if applicable, brief (one line) description of one of the
// function's arguments>
// * @param erase_all
// * 		<if applicable, brief (one line) description of one of the
// function's arguments>
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_erase_file(File_Type file_type, uint8_t file_counter,
// bool erase_all) {
//  uint32_t command[4];
//  command[0] = ERASE_FILE_ID;
//  command[1] = file_type;
//  command[2] = file_counter;
//  command[3] = erase_all;
//  return adcs_telecommand(command, 4);
//}
//
///**
// * @brief
// * 		Fill download with file contents
// * @param file_type
// * 		Accepted parameters: FILE_TYPE_TELEMETERY_LOG,
// FILE_TYPE_JPG_IMAGE,
// *                          FILE_TYPE_BMP_IMAGE or FILE_TYPE_INDEX.
// * @param counter
// * 		<if applicable, brief (one line) description of one of the
// function's arguments>
// * @param offset
// * 		<if applicable, brief (one line) description of one of the
// function's arguments>
// * @param block_length
// * 		<if applicable, brief (one line) description of one of the
// function's arguments>
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_load_file_download_block(File_Type file_type, uint8_t
// counter, uint32_t offset, uint16_t block_length){
//  uint32_t command[5];
//  command[0] = LOAD_FILE_DOWNLOAD_BLOCK_ID;
//  command[1] = file_type;
//  command[2] = counter;
//  command[3] = offset;
//  command[4] = block_length;
//
//  return adcs_telecommand(command, 5);
//}
//
///**
// * @brief
// * 		Advance File List Read Pointer
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_advance_file_list_read_pointer(){
//  uint32_t command = ADVANCE_FILE_LIST_READ_POINTER_ID;
//  return adcs_telecommand(&command, 1);
//}
//
///**
// * @brief
// * 		Initiate File Upload
// * @param file_dest
// * 		Accepted parameters: EEPROM, FLASH_PROGRAM_1 - FLASH_PROGRAM_7,
// *                          SD_USER_FILE_1 - SD_USER_FILE_8
// * @param block_size
// *
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_initiate_file_upload(File_Upload_Dest file_dest,
// uint8_t block_size){
//  uint32_t command[3];
//  command[0] = INITIATE_FILE_UPLOAD_ID;
//  command[1] = file_dest;
//  command[2] = block_size;
//  adcs_telecommand(command, 3);
//}
//
///**
// * @brief
// * 		File Upload Packet
// * @param packet_number
// * 		Packet Number
// * @param file_bytes
// * 		File Bytes
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_file_upload_packet(uint16_t packet_number, char
// *file_bytes){
//  uint32_t command[3];
//  command[0] = FILE_UPLOAD_PACKET_ID;
//  command[1] = packet_number;
//  command[2] = file_bytes;
//  adcs_telecommand(command, 3);
//}
//
///**
// * @brief
// * 		Finalize Uploaded File Block
// * @param file_dest
// * 		File Destination
// * @param offset
// * 		Offset into file
// * @param block_length
// * 		Length of block
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_finalize_upload_block(File_Upload_Dest file_dest,
// uint32_t offset, uint16_t block_length){
//  uint32_t command[4];
//  command[0] = FINALIZE_UPLOAD_BLOCK_ID;
//  command[1] = file_dest;
//  command[2] = offset;
//  command[3] = block_length;
//  adcs_telecommand(command, 4);
//}
//
///**
// * @brief
// * 		Reset HoleMap for Upload Block
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_reset_upload_block(){
//  uint32_t command = RESET_UPLOAD_BLOCK_ID;
//  return adcs_telecommand(&command, 1);
//}
//
///**
// * @brief
// * 		Reset File List Read Pointer
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_reset_file_list_read_pointer(){
//  uint32_t command = RESET_FILE_LIST_READ_POINTER_ID;
//  return adcs_telecommand(&command, 1);
//}
//
///**
// * @brief
// * 		Initiate Download Burst
// * @param msg_length
// * 		Message Length
// * @param ignore_hole_map
// * 		Ignore Hole Map
// * @return
// * 		<if applicable, brief description of what is returned by the
// function>
// */
// ADCS_returnState ADCS_initiate_download_burst(uint8_t msg_length, bool
// ignore_hole_map){
//  uint32_t command[3];
//  command[0] = INITIATE_DOWNLOAD_BURST_ID;
//  command[1] = msg_length;
//  command[2] = ignore_hole_map;
//  adcs_telecommand(command, 3);
//}

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
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_run_selected_program(
    void) {  //* could be merged with the previous one in services
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
ADCS_returnState ADCS_read_program_info(
    uint8_t index) {  //* why would we need the enum
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
 * 		Table 66 - Source Program index				//* not
 * good for services 0 : BootLoader, 1 : Internal flash program, 2 : EEPROM,
 * 3-9: External flash program 1-7, 10-17: SD user file 1-8
 * @attention
 * 		flag = 0x5A overwrites the boot segment
 * @return
 * 		Success of function defined in adcs_types.h
 */
ADCS_returnState ADCS_copy_program_internal_flash(
    uint8_t index, uint8_t overwrite_flag) {  //* why would we need the enum
  uint8_t command[3];
  command[0] = COPY_PROGRAM_INTERNAL_FLASH_ID;
  if (index < 0 || index > 18) {
    return ADCS_INVALID_PARAMETERS;
  } else {
    command[1] = index;
    //* In QB50 it's unused. Would we want to overwrite boot segment?
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
  *busy = telemetry[1] & 1;  //*
  // Different endianness! Should we use conversion functions?
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
