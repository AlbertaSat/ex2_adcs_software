#include "unity.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "adcs_handler.h"
#include "mock_adcs_io.h"

uint8_t TC_ID_array[] = {1,4,5,6,33,108,112,113,114,115,116,117,118,119};

void setUp(void)
{
}

void tearDown(void)
{
}

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

    // command = realloc(command, sizeof(*command)*3);
    // uint16_t pnum   = 2;
    // char    *fileb  = "file_name";
    // command[0] = TC_ID_array[9];
    // command[1] = pnum;
    // command[2] = fileb;
    // send_uart_telecommand_ExpectAndReturn(command, 1, ADCS_OK);
    // TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_file_upload_packet(pnum, fileb));

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
    
}

void test_ADCS_common_telemetry(void)
{
    TEST_IGNORE_MESSAGE("Need to Implement ADCS_CommonTelemetry");
}

