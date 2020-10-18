#include "unity.h"


#include "adcs_handler.h"
#include "mock_adcs_io.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_ADCS_common_commands(void)
{  

    //ADCS_OK
    send_uart_telecommand_IgnoreAndReturn(ADCS_OK);
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_reset());
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_reset_log_pointer());
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_advance_log_pointer());
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_reset_boot_registers());
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_format_sd_card());
    TEST_ASSERT_EQUAL_INT(ADCS_OK, ADCS_erase_file(FILE_TYPE_INDEX, 1, 1));

    //ADCS_invalid_TC_ID
    
}

void test_ADCS_common_telemetry(void)
{
    TEST_IGNORE_MESSAGE("Need to Implement ADCS_CommonTelemetry");
}