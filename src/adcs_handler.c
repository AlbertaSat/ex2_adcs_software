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
#include "adcs_io.h"

#define USE_UART
//#define USE_I2C


/**
 * @brief
 * 		
 * @details
 * 		<more detailed description of the function>
 * @return
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState adcs_telecommand(uint32_t * command, uint32_t length) {
  //send and receive data via selected data protocol (i2c, SPI, UART)

  // //Send SOM bytes 
  // uart_send(1, &ESC_CHAR);
  // uart_send(1, &SOM);

  // //Send Command
  // uart_send(length, command);

  // //Send EOM
  // uart_send(1, &ESC_CHAR);
  // uart_send(1, &EOM);

  //get Acknowledgement 
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
 * 		Perform a reset.
 * @details
 * 		<more detailed description of the function>
 * @return
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_reset() {
  uint32_t command[2];
  command[0] = RESET_ID;
  command[1] = ADCS_MAGIC_NUMBER; // Magic number
  return adcs_telecommand(command, 2);
}

/**
 * @brief
 * 		Reset pointer to log buffer (from where LastLogEvent TLM is returned)
 * @details
 * 		<more detailed description of the function>
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_reset_log_pointer() {
  uint32_t command = RESET_LOG_POINTER_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Advance pointer to log buffer (from where LastLogEvent TLM is returned)
 * @details
 * 		<more detailed description of the function>
 * @return
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_advance_log_pointer() {
  uint32_t command = ADVANCE_LOG_POINTER_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Reset Boot Registers
 * @details
 * 		<more detailed description of the function>
 * @return
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_reset_boot_registers() {
  uint32_t command = RESET_BOOT_REGISTERS_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Format SD Card
 * @details
 * 		<more detailed description of the function>
 * @return
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_format_sd_card() {
  uint32_t command[2];
  command[0] = FORMAT_SD_CARD_ID;
  command[1] = ADCS_MAGIC_NUMBER; // magic number
  return adcs_telecommand(command, 2);
}

/**
 * @brief
 * 		Erase file
 * @details
 * 		<more detailed description of the function>
 * @param file_type
 * 		Accepted parameters: FILE_TYPE_TELEMETERY_LOG, FILE_TYPE_JPG_IMAGE, 
 *                          FILE_TYPE_BMP_IMAGE or FILE_TYPE_INDEX.
 * @param file_counter
 * 		<if applicable, brief (one line) description of one of the function's arguments>
 * @param erase_all
 * 		<if applicable, brief (one line) description of one of the function's arguments>
 * @return
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_erase_file(File_Type file_type, uint8_t file_counter, bool erase_all) {
  uint32_t command[4];
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
 * 		Accepted parameters: FILE_TYPE_TELEMETERY_LOG, FILE_TYPE_JPG_IMAGE, 
 *                          FILE_TYPE_BMP_IMAGE or FILE_TYPE_INDEX.
 * @param counter
 * 		<if applicable, brief (one line) description of one of the function's arguments>
 * @param offset
 * 		<if applicable, brief (one line) description of one of the function's arguments>
 * @param block_length
 * 		<if applicable, brief (one line) description of one of the function's arguments>
 * @return
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_load_file_download_block(File_Type file_type, uint8_t counter, uint32_t offset, uint16_t block_length){
  uint32_t command[5];
  command[0] = LOAD_FILE_DOWNLOAD_BLOCK_ID;
  command[1] = file_type;
  command[2] = counter;
  command[3] = offset;
  command[4] = block_length;

  return adcs_telecommand(command, 5);
}

/**
 * @brief
 * 		Advance File List Read Pointer
 * @return
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_advance_file_list_read_pointer(){
  uint32_t command = ADVANCE_FILE_LIST_READ_POINTER_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Initiate File Upload
 * @param file_dest
 * 		Accepted parameters: EEPROM, FLASH_PROGRAM_1 - FLASH_PROGRAM_7,
 *                          SD_USER_FILE_1 - SD_USER_FILE_8
 * @param block_size
 * 
 * @return
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_initiate_file_upload(File_Upload_Dest file_dest, uint8_t block_size){
  uint32_t command[3];
  command[0] = INITIATE_FILE_UPLOAD_ID;
  command[1] = file_dest;
  command[2] = block_size;
  adcs_telecommand(command, 3);
}

/**
 * @brief
 * 		File Upload Packet
 * @param packet_number
 * 		Packet Number
 * @param file_bytes
 * 		File Bytes
 * @return
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_file_upload_packet(uint16_t packet_number, char *file_bytes){
  uint32_t command[3];
  command[0] = FILE_UPLOAD_PACKET_ID;
  command[1] = packet_number;
  command[2] = file_bytes;
  adcs_telecommand(command, 3);
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
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_finalize_upload_block(File_Upload_Dest file_dest, uint32_t offset, uint16_t block_length){
  uint32_t command[4];
  command[0] = FINALIZE_UPLOAD_BLOCK_ID;
  command[1] = file_dest;
  command[2] = offset;
  command[3] = block_length;
  adcs_telecommand(command, 4);
}

/**
 * @brief
 * 		Reset HoleMap for Upload Block
 * @return
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_reset_upload_block(){
  uint32_t command = RESET_UPLOAD_BLOCK_ID;
  return adcs_telecommand(&command, 1);
}

/**
 * @brief
 * 		Reset File List Read Pointer
 * @return
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_reset_file_list_read_pointer(){
  uint32_t command = RESET_FILE_LIST_READ_POINTER_ID;
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
 * 		<if applicable, brief description of what is returned by the function>
 */
ADCS_returnState ADCS_initiate_download_burst(uint8_t msg_length, bool ignore_hole_map){
  uint32_t command[3];
  command[0] = INITIATE_DOWNLOAD_BURST_ID;
  command[1] = msg_length;
  command[2] = ignore_hole_map;
  adcs_telecommand(command, 3);
}

