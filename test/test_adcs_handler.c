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

#include "unity.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "adcs_handler.h"
#include "adcs_io.h"
#include "mock_uart_i2c.h"


uint8_t TC_ID_array[] = {1,4,5,6,33,108,112,113,114,115,116,117,118,119};

void setUp(void)
{
}

void tearDown(void)
{
}
/*
void test_ADCS_common_commands(void)
{  
    uint32_t *command = NULL;

    //ADCS_OK
    command = realloc(command, sizeof(*command)*2);
    command[0] = TC_ID_array[0], 
    command[1] = 0x5A;
    send_uart_telecommand_ExpectAndReturn(command, 2, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_reset());

    command = realloc(command, sizeof(*command)*1);
    command[0] = TC_ID_array[1];
    send_uart_telecommand_ExpectAndReturn(command, 1, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_reset_log_pointer());

    command = realloc(command, sizeof(*command)*1);
    command[0] = TC_ID_array[2];
    send_uart_telecommand_ExpectAndReturn(command, 1, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_advance_log_pointer());

    command = realloc(command, sizeof(*command)*1);
    command[0] = TC_ID_array[3];
    send_uart_telecommand_ExpectAndReturn(command, 1, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_reset_boot_registers());

    command = realloc(command, sizeof(*command)*2);
    command[0] = TC_ID_array[4]; 
    command[1] = 0x5A;
    send_uart_telecommand_ExpectAndReturn(command, 2, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_format_sd_card());

    command = realloc(command, sizeof(*command)*4);
    File_Type ft = FILE_TYPE_INDEX;
    uint8_t   ct = 1;
    bool   ea = true;
    command[0] = TC_ID_array[5]; 
    command[1] = ft;
    command[2] = ct;
    command[3] = ea;
    send_uart_telecommand_ExpectAndReturn(command, 4, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_erase_file(ft, ct, ea));

    command = realloc(command, sizeof(*command)*5);
    File_Type ftype   = FILE_TYPE_INDEX;
    uint8_t   counter = 1;
    uint32_t  offset  = 1;
    uint16_t  blength = 1;
    command[0] = TC_ID_array[6];
    command[1] = ftype;
    command[2] = counter;
    command[3] = offset;
    command[4] = blength;
    send_uart_telecommand_ExpectAndReturn(command, 5, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_load_file_download_block(ftype, counter, offset, blength));

    command = realloc(command, sizeof(*command)*1);
    command[0] = TC_ID_array[7];
    send_uart_telecommand_ExpectAndReturn(command, 1, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_advance_file_list_read_pointer());

    command = realloc(command, sizeof(*command)*3);
    File_Upload_Dest fdest = EEPROM;
    uint8_t bsize          = 2;
    command[0]             = TC_ID_array[8];
    command[1]             = fdest;
    command[2]             = bsize;
    send_uart_telecommand_ExpectAndReturn(command, 3, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_initiate_file_upload(fdest, bsize));

     command = realloc(command, sizeof(*command)*3);
     uint16_t pnum   = 2;
     char    *fileb  = "file_name";
     command[0] = TC_ID_array[9];
     command[1] = pnum;
     command[2] = fileb;
     send_uart_telecommand_ExpectAndReturn(command, 3, ADCS_OK);
     TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_file_upload_packet(pnum, fileb));

    command = realloc(command, sizeof(*command)*4);
    fdest = EEPROM;
    offset  = 2;
    blength = 1;
    command[0] = TC_ID_array[10];
    command[1] = fdest;
    command[2] = offset;
    command[3] = blength;
    send_uart_telecommand_ExpectAndReturn(command, 4, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_finalize_upload_block(fdest, offset, blength));

    command = realloc(command, sizeof(*command)*1);
    command[0] = TC_ID_array[11];
    send_uart_telecommand_ExpectAndReturn(command, 1, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_reset_upload_block());

    command = realloc(command, sizeof(*command)*1);
    command[0] = TC_ID_array[12];
    send_uart_telecommand_ExpectAndReturn(command, 1, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_reset_file_list_read_pointer());

    command = realloc(command, sizeof(*command)*3);
    uint8_t msg_l = 8;
    bool    ign_hole = true;
    command[0] = TC_ID_array[13];
    command[1] = msg_l;
    command[2] = ign_hole;
    send_uart_telecommand_ExpectAndReturn(command, 3, ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_initiate_download_burst(msg_l, ign_hole));
    //ADCS_invalid_TC_ID
    
    free(command);
    command = NULL;
}*/

void test_ADCS_common_telemetry(void)
{
    TEST_IGNORE_MESSAGE("Need to Implement ADCS_CommonTelemetry");
}

void test_ADCS_BootLoader_telecommands(void)
{
	// Same for all uart TC replies
	uint8_t reply[6];
	reply[0] = ADCS_ESC_CHAR;
	reply[1] = ADCS_SOM;
	reply[2] = CLEAR_ERR_FLAGS_ID;
	reply[3] = ADCS_OK;
	reply[4] = ADCS_ESC_CHAR;
	reply[5] = ADCS_EOM;

	// Doesn't matter what we put in frame!
	uint8_t *frame = NULL;
	frame = realloc(frame, sizeof(*frame)*5);
	frame[0] = ADCS_ESC_CHAR;
	frame[1] = ADCS_SOM;
	frame[2] = CLEAR_ERR_FLAGS_ID;
	frame[3] = ADCS_ESC_CHAR;
	frame[4] = ADCS_EOM;
	uart_send_Expect(frame, 5);
	uart_receive_ExpectAnyArgs();
	uart_receive_ReturnArrayThruPtr_data(reply, 6);
	TEST_ASSERT_EQUAL_INT(ADCS_OK,ADCS_clear_err_flags());

	// Set boot index
	frame = realloc(frame, sizeof(*frame)*6);
	frame[0] = ADCS_ESC_CHAR;
	frame[1] = ADCS_SOM;
	frame[2] = SET_BOOT_INDEX_ID;
	frame[3] = 1;
	frame[4] = ADCS_ESC_CHAR;
	frame[5] = ADCS_EOM;
	uart_send_Expect(frame, 6);
	uart_receive_ExpectAnyArgs();
	uart_receive_ReturnArrayThruPtr_data(reply, 6);
	TEST_ASSERT_EQUAL_INT(ADCS_OK,ADCS_set_boot_index(1));// only argument 1 passes

}


void test_ADCS_BootLoader_telemetries(void)
{
	//BootLoader state
	uint8_t request[5];
	request[0] = ADCS_ESC_CHAR;
	request[1] = ADCS_SOM;
	request[2] = GET_BOOTLOADER_STATE_ID;
	request[3] = ADCS_ESC_CHAR;
	request[4] = ADCS_EOM;
	uint8_t *reply = NULL;

	uint16_t uptime = 7785;
	uint32_t flags = 0x0127; //hex presentation to see it better
	reply = realloc(reply, sizeof(*reply)*11);
	reply[0] = ADCS_ESC_CHAR;
	reply[1] = ADCS_SOM;
	reply[2] = GET_BOOTLOADER_STATE_ID;
	memcpy(&reply[3], &uptime, 2);
	memcpy(&reply[5], &flags, 4);
	reply[9] = ADCS_ESC_CHAR;
	reply[10] = ADCS_EOM;

	uint16_t uptime_f;
	uint8_t flags_arr[11];
	uart_send_Expect(request, 5);
	uart_receive_ExpectAnyArgs();
	uart_receive_ReturnArrayThruPtr_data(reply, 11);
	ADCS_get_bootloader_state(&uptime_f, flags_arr);
	TEST_ASSERT_EQUAL_UINT16(uptime_f,uptime);
	uint8_t flags_f[11] = {1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0}; // 7 2 1
	TEST_ASSERT_EQUAL_UINT8_ARRAY(flags_arr, flags_f, 11);

	// Program info
	uint8_t index = 5;
	uint8_t busy = 1;
	uint32_t file_size = 14569;
	uint16_t crc16_checksum = 3172;
	reply = realloc(reply, sizeof(*reply)*13);
	reply[0] = ADCS_ESC_CHAR;
	reply[1] = ADCS_SOM;
	reply[2] = GET_PROGRAM_INFO_ID;
	reply[3] = index;
	reply[4] = busy;
	memcpy(&reply[5], &file_size, 4);
	memcpy(&reply[9], &crc16_checksum, 2);
	reply[11] = ADCS_ESC_CHAR;
	reply[12] = ADCS_EOM;
	uint8_t index_f;
	bool busy_f;
	uint32_t file_size_f;
	uint16_t crc16_checksum_f;
	request[2] = GET_PROGRAM_INFO_ID;
	uart_send_Expect(request, 5);
	uart_receive_ExpectAnyArgs();
	uart_receive_ReturnArrayThruPtr_data(reply, 13);
	ADCS_get_program_info(&index_f, &busy_f, &file_size_f, &crc16_checksum_f);
	TEST_ASSERT_EQUAL_UINT8(index,index_f);
	TEST_ASSERT_EQUAL_INT(1,busy_f);
	TEST_ASSERT_EQUAL_UINT32(file_size,file_size_f);
	TEST_ASSERT_EQUAL_UINT16(crc16_checksum,crc16_checksum_f);
}
