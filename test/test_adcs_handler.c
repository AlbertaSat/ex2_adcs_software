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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "adcs_handler.h"
#include "adcs_io.h"
#include "mock_uart_i2c.h"
#include "unity.h"

uint8_t TC_ID_array[] = {1,   4,   5,   6,   33,  108, 112,
                         113, 114, 115, 116, 117, 118, 119};

void setUp(void) {}

void tearDown(void) {}

void test_ADCS_common_commands(void) {
  // Same for all uart TC replies
  uint8_t reply[6];
  reply[0] = ADCS_ESC_CHAR;
  reply[1] = ADCS_SOM;
  reply[2] = LOAD_FILE_DOWNLOAD_BLOCK_ID;
  reply[3] = ADCS_OK;
  reply[4] = ADCS_ESC_CHAR;
  reply[5] = ADCS_EOM;

  // load file
  uart_send_Ignore();
  uart_receive_ExpectAnyArgs();
  uart_receive_ReturnArrayThruPtr_data(reply, 6);
  TEST_ASSERT_EQUAL_INT(ADCS_OK,
                        ADCS_load_file_download_block(2, 17, 6324, 145));

  // file upload
  uart_send_Ignore();
  uart_receive_ExpectAnyArgs();
  uart_receive_ReturnArrayThruPtr_data(reply, 6);
  TEST_ASSERT_EQUAL_INT(ADCS_OK,
                        ADCS_file_upload_packet(145, "This is test file"));
}

void test_ADCS_common_telemetry(void) {
  TEST_IGNORE_MESSAGE("Need to Implement ADCS_CommonTelemetry");
}

void test_ADCS_BootLoader_telecommands(void) {
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
  frame = realloc(frame, sizeof(*frame) * 5);
  frame[0] = ADCS_ESC_CHAR;
  frame[1] = ADCS_SOM;
  frame[2] = CLEAR_ERR_FLAGS_ID;
  frame[3] = ADCS_ESC_CHAR;
  frame[4] = ADCS_EOM;
  uart_send_Expect(frame, 5);
  uart_receive_ExpectAnyArgs();
  uart_receive_ReturnArrayThruPtr_data(reply, 6);
  TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_clear_err_flags());

  // Set boot index
  frame = realloc(frame, sizeof(*frame) * 6);
  frame[0] = ADCS_ESC_CHAR;
  frame[1] = ADCS_SOM;
  frame[2] = SET_BOOT_INDEX_ID;
  frame[3] = 1;
  frame[4] = ADCS_ESC_CHAR;
  frame[5] = ADCS_EOM;
  uart_send_Expect(frame, 6);
  uart_receive_ExpectAnyArgs();
  uart_receive_ReturnArrayThruPtr_data(reply, 6);
  TEST_ASSERT_EQUAL_INT(ADCS_OK,
                        ADCS_set_boot_index(1));  // only argument 1 passes

  free(frame);
}

void test_ADCS_BootLoader_telemetries(void) {
  // BootLoader state
  uint8_t request[5];
  request[0] = ADCS_ESC_CHAR;
  request[1] = ADCS_SOM;
  request[2] = GET_BOOTLOADER_STATE_ID;
  request[3] = ADCS_ESC_CHAR;
  request[4] = ADCS_EOM;
  uint8_t *reply = NULL;

  uint16_t uptime = 7785;
  uint32_t flags = 0x0127;  // hex presentation to see it better
  reply = realloc(reply, sizeof(*reply) * 11);
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
  TEST_ASSERT_EQUAL_UINT16(uptime_f, uptime);
  uint8_t flags_f[11] = {1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0};  // 7 2 1
  TEST_ASSERT_EQUAL_UINT8_ARRAY(flags_arr, flags_f, 11);

  // Program info
  uint8_t index = 5;
  uint8_t busy = 1;
  uint32_t file_size = 14569;
  uint16_t crc16_checksum = 3172;
  reply = realloc(reply, sizeof(*reply) * 13);
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
  TEST_ASSERT_EQUAL_UINT8(index, index_f);
  TEST_ASSERT_EQUAL_INT(1, busy_f);
  TEST_ASSERT_EQUAL_UINT32(file_size, file_size_f);
  TEST_ASSERT_EQUAL_UINT16(crc16_checksum, crc16_checksum_f);

  free(reply);
}

void test_ADCS_ACP_telecommands(void) {
  // Same for all uart TC replies
  uint8_t reply[6];
  reply[0] = ADCS_ESC_CHAR;
  reply[1] = ADCS_SOM;
  reply[2] = CLEAR_LATCHED_ERRS_ID;
  reply[3] = ADCS_OK;
  reply[4] = ADCS_ESC_CHAR;
  reply[5] = ADCS_EOM;

  // Clear latched errors
  uart_send_Ignore();
  uart_receive_ExpectAnyArgs();
  uart_receive_ReturnArrayThruPtr_data(reply, 6);
  TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_clear_latched_errs(1, 1));

  // Set attitude control
  uart_send_Ignore();
  uart_receive_ExpectAnyArgs();
  uart_receive_ReturnArrayThruPtr_data(reply, 6);
  TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_set_attitude_ctrl_mode(14, 6589));

  // set magnetorquer output
  uart_send_Ignore();
  uart_receive_ExpectAnyArgs();
  uart_receive_ReturnArrayThruPtr_data(reply, 10);
  TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_set_magnetorquer_output(630, 786, 912));
}
