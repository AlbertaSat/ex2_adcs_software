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

#ifndef ADCS_HANDLER_H
#define ADCS_HANDLER_H

#include <stdbool.h>
#include <stdint.h>

#include "adcs_types.h"

// send_telecommand
ADCS_returnState adcs_telecommand(uint8_t *command, uint32_t length);
ADCS_returnState adcs_telemetry(uint8_t TM_ID, uint8_t *reply, uint32_t length);

// Common Telecommands
ADCS_returnState ADCS_reset();
ADCS_returnState ADCS_reset_log_pointer();
ADCS_returnState ADCS_advance_log_pointer();
ADCS_returnState ADCS_reset_boot_registers();
ADCS_returnState ADCS_format_sd_card();
ADCS_returnState ADCS_erase_file(File_Type file_type, uint8_t file_counter,
                                 bool erase_all);
ADCS_returnState ADCS_load_file_download_block(File_Type file_type,
                                               uint8_t counter, uint32_t offset,
                                               uint16_t block_length);
ADCS_returnState ADCS_advance_file_list_read_pointer();
ADCS_returnState ADCS_initiate_file_upload(File_Upload_Dest file_dest,
                                           uint8_t block_size);
ADCS_returnState ADCS_file_upload_packet(
    uint16_t packet_number,
    char *file_bytes);  //***********************************
ADCS_returnState ADCS_finalize_upload_block(File_Upload_Dest file_dest,
                                            uint32_t offset,
                                            uint16_t block_length);
ADCS_returnState ADCS_reset_upload_block();
ADCS_returnState ADCS_reset_file_list_read_pointer();
ADCS_returnState ADCS_initiate_download_burst(uint8_t msg_length,
                                              bool ignore_hole_map);

// Common Telemetry

// BootLoader Telecommands
ADCS_returnState ADCS_clear_err_flags(void);
ADCS_returnState ADCS_set_boot_index(uint8_t index);
ADCS_returnState ADCS_run_selected_program(void);
ADCS_returnState ADCS_read_program_info(uint8_t index);
ADCS_returnState ADCS_copy_program_internal_flash(uint8_t index,
                                                  uint8_t overwrite_flag);

// BootLoader Telemetries
ADCS_returnState ADCS_get_bootloader_state(uint16_t *uptime,
                                           uint8_t *flags_arr);
ADCS_returnState ADCS_get_program_info(uint8_t *index, bool *busy,
                                       uint32_t *file_size,
                                       uint16_t *crc16_checksum);
ADCS_returnState ADCS_copy_internal_flash_progress(bool *busy, bool *err);

// ACP Telecommands
ADCS_returnState ADCS_deploy_magnetometer_boom(uint8_t actuation_timeout);
ADCS_returnState ADCS_set_enabled_state(uint8_t state);
ADCS_returnState ADCS_clear_latched_errs(bool adcs_flag, bool hk_flag);
ADCS_returnState ADCS_set_attitude_ctrl_mode(uint8_t ctrl_mode,
                                             uint16_t timeout);
ADCS_returnState ADCS_set_attitude_estimate_mode(uint8_t mode);
ADCS_returnState ADCS_trigger_adcs_loop(void);
ADCS_returnState ADCS_trigger_adcs_loop_sim(uint8_t *sim_data);
ADCS_returnState ADCS_set_ASGP4_rune_mode(uint8_t mode);
ADCS_returnState ADCS_trigger_ASGP4(void);
ADCS_returnState ADCS_set_MTM_op_mode(uint8_t mode);
ADCS_returnState ADCS_cnv2jpg(uint8_t source, uint8_t QF,
                              uint8_t white_balance);
ADCS_returnState ADCS_save_img(uint8_t camera, uint8_t img_size);
ADCS_returnState ADCS_set_magnetorquer_output(int16_t x, int16_t y, int16_t z);
ADCS_returnState ADCS_set_wheel_speed(int16_t x, int16_t y, int16_t z);
ADCS_returnState ADCS_save_config(void);
ADCS_returnState ADCS_save_orbit_params(void);

#endif /* ADCS_HANDLER_H */
