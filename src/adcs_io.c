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
* @file adcs_io.c
* @author Andrew Rooney
* @date 2020-08-09
*/

#include <adcs_handler.h>
#include <adcs_types.h>

static i2cBASE_t* i2c;
static uint8_t I2C_Write_Addr 0xAE;
static uint8_t I2C_Read_Addr 0xAF;

void adcs_i2c_init(i2cBASE_t* i2c_reg) {
  i2c = i2c_reg;
  i2cSetMode(i2c, I2C_MASTER);
  return;
}

ADCS_returnState adcs_telecommand(uint8_t* command, uint8_t length) {
  return ADCS_ok;
}

void adcs_i2c_telecommand(uint8_t* command, uint8_t length) {
  i2cSetSlaveAdd(i2c, I2C_Write_Addr);
  i2cSetDirection(i2c, I2C_TRANSMITTER);
  i2cSetStart(i2c);
  i2cSendByte(i2c, I2C_Write_Addr); // send write address and then command ID+data
  i2cSend(i2c, (uint32_t) length, command);
  i2cSetStop(i2c);
  // TODO: get the Acknowledgement
  return;
}
