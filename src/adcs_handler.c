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
ADCS_returnState adcs_telecommand(uint8_t * command, uint8_t length) {
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
    /*
     *--------------------MOCK-------------------------------------
     */
    
    /*
     *-------------------------------------------------------------
     */
    ack = send_uart_telecommand(command, length);

  #elif USE_I2C

    /*
     *--------------------MOCK-------------------------------------
     */
    adcs_i2c_telecommand_ExpectAnyArgsAndReturn(ADCS_OK);
    /*
     *-------------------------------------------------------------
     */
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
  uint8_t command[2];
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
  uint8_t command = RESET_LOG_POINTER_ID;
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
  uint8_t command = ADVANCE_LOG_POINTER_ID;
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
  uint8_t command = RESET_BOOT_REGISTERS_ID;
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
  uint8_t command[2];
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
ADCS_returnState ADCS_erase_file(File_Type file_type, uint8_t file_counter, uint8_t erase_all) {
  uint8_t command[4];
  command[0] = ERASE_FILE_ID;
  command[1] = file_type;
  command[2] = file_counter;
  command[3] = erase_all;
  return adcs_telecommand(command, 4);
}
