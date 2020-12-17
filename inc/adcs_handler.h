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

// Structs
typedef struct {
  float x;
  float y;
  float z;
} xyz;

typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
} xyz16;

typedef struct {
  uint8_t x;
  uint8_t y;
  uint8_t z;
} xyzu8;

typedef struct {
  uint8_t att_estimate_mode;
  uint8_t att_ctrl_mode;
  uint8_t run_mode;
  uint8_t ASGP4_mode;
  uint8_t flags_arr[52];
  uint8_t MTM_sample_mode;
  xyz est_angle;  // est: estimated
  xyz16 est_quaternion;
  xyz est_angular_rate;
  xyz ECI_pos;
  xyz ECI_vel;
  xyz longlatalt;
  xyz16 ecef_pos;
} adcs_state;

typedef struct {
  xyz magnetic_field;
  xyz coarse_sun;
  xyz sun;
  xyz nadir;
  xyz angular_rate;
  xyz wheel_speed;
  xyz star1b;
  xyz star1o;
  xyz star2b;
  xyz star2o;
  xyz star3b;
  xyz star3o;
} adcs_measures;

typedef struct {
  xyz magnetorquer;
  xyz wheel_speed;
} adcs_actuator;

typedef struct {
  xyz igrf_magnetic_field;
  xyz sun;
  xyz gyro_bias;
  xyz innovation;
  xyz quaternion_err;
  xyz quaternion_covar;
  xyz angular_rate_covar;
} adcs_estimate;

typedef struct {
  float epoch;
  float inclination;
  float RAAN;
  float ECC;
  float AOP;
  float MA;
  float MM;
  float Bstar;
} adcs_asgp4;

typedef struct {
  int16_t centroid_x;
  int16_t centroid_y;
  uint8_t capture_stat;
  uint8_t detect_result;
} cam_sensor;

typedef struct {
  cam_sensor cam1;
  cam_sensor cam2;
  uint8_t css[10];
  xyz16 MTM;
  xyz16 rate;
} adcs_raw_sensor;

typedef struct {
  int32_t pos;
  int16_t vel;
} ecef;

typedef struct {
  uint8_t sol_stat;
  uint8_t tracked_sats;
  uint8_t usedInSol_sats;
  uint8_t xyz_lof_count;
  uint8_t range_log_count;
  uint8_t response_msg;
  uint16_t reference_week;
  uint32_t time;  // ms
  ecef x;
  ecef y;
  ecef z;
  xyz pos_std_dev;
  xyzu8 vel_std_dev;
} adcs_raw_gps;

typedef struct {
  uint8_t confidence;
  uint16_t magnitude;
  uint16_t catalouge_num;
  int16_t centroid_x;
  int16_t centroid_y;
} star_data;

typedef struct {
  uint8_t detected_stars;
  uint8_t img_noise;
  uint8_t invalid_stars;
  uint8_t identified_stars;
  uint8_t identification_mode;  // Table 147
  uint8_t img_dark_val;
  uint8_t flags_arr[8];
  uint16_t sample_T;
  star_data star1;
  star_data star2;
  star_data star3;
  uint16_t capture_t;
  uint16_t detect_t;
  uint16_t identification_t;
  xyz estimated_rate;
  xyz estimated_att;
} adcs_star_track;

typedef struct {
  float cubesense1_3v3_I;  // I : current
  float cubesense1_camSram_I;
  float cubesense2_3v3_I;
  float cubesense2_camSram_I;
  float cubecontrol_3v3_I;
  float cubecontrol_5v_I;
  float cubecontrol_vBat_I;
  float wheel1_I;
  float wheel2_I;
  float wheel3_I;
  float cubestar_I;
  float magnetorquer_I;
  float cubestar_temp;  // temperature
  float MCU_temp;
  float MTM_temp;
  float MTM2_temp;
  xyz16 rate_sensor_temp;
} adcs_pwr_temp;

// General functions
int16_t uint82int16(uint8_t b1, uint8_t b2);
int32_t uint82int32(uint8_t* address);
uint16_t uint82uint16(uint8_t b1, uint8_t b2);
void get_xyz(xyz* measurement, uint8_t* address, float coef);
void get_xyz16(xyz16* measurement, uint8_t* address);

// send_telecommand
ADCS_returnState adcs_telecommand(uint8_t* command, uint32_t length);
ADCS_returnState adcs_telemetry(uint8_t TM_ID, uint8_t* reply, uint32_t length);

// Common Telecommands
ADCS_returnState ADCS_reset(void);
ADCS_returnState ADCS_reset_log_pointer(void);
ADCS_returnState ADCS_advance_log_pointer(void);
ADCS_returnState ADCS_reset_boot_registers(void);
ADCS_returnState ADCS_format_sd_card(void);
ADCS_returnState ADCS_erase_file(File_Type file_type, uint8_t file_counter,
                                 bool erase_all);
ADCS_returnState ADCS_load_file_download_block(File_Type file_type,
                                               uint8_t counter, uint32_t offset,
                                               uint16_t block_length);
ADCS_returnState ADCS_advance_file_list_read_pointer(void);
ADCS_returnState ADCS_initiate_file_upload(File_Upload_Dest file_dest,
                                           uint8_t block_size);
ADCS_returnState ADCS_file_upload_packet(uint16_t packet_number,
                                         char* file_bytes);
ADCS_returnState ADCS_finalize_upload_block(File_Upload_Dest file_dest,
                                            uint32_t offset,
                                            uint16_t block_length);
ADCS_returnState ADCS_reset_upload_block(void);
ADCS_returnState ADCS_reset_file_list_read_pointer(void);
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
ADCS_returnState ADCS_get_bootloader_state(uint16_t* uptime,
                                           uint8_t* flags_arr);
ADCS_returnState ADCS_get_program_info(uint8_t* index, bool* busy,
                                       uint32_t* file_size,
                                       uint16_t* crc16_checksum);
ADCS_returnState ADCS_copy_internal_flash_progress(bool* busy, bool* err);

// ACP Telecommands
ADCS_returnState ADCS_deploy_magnetometer_boom(uint8_t actuation_timeout);
ADCS_returnState ADCS_set_enabled_state(uint8_t state);
ADCS_returnState ADCS_clear_latched_errs(bool adcs_flag, bool hk_flag);
ADCS_returnState ADCS_set_attitude_ctrl_mode(uint8_t ctrl_mode,
                                             uint16_t timeout);
ADCS_returnState ADCS_set_attitude_estimate_mode(uint8_t mode);
ADCS_returnState ADCS_trigger_adcs_loop(void);
ADCS_returnState ADCS_trigger_adcs_loop_sim(uint8_t* sim_data);
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

// ACP Telemetry
ADCS_returnState ADCS_get_current_state(adcs_state* data);

ADCS_returnState ADCS_get_jpg_cnv_progress(uint8_t* percentage, uint8_t* result,
                                           uint8_t* file_counter);
ADCS_returnState ADCS_get_cubeACP_state(uint8_t* flags_arr);
ADCS_returnState ADCS_get_execution_times(uint8_t* adcs_update,
                                          uint8_t* sensor_comms,
                                          uint8_t* sgp4_propag,
                                          uint8_t* igrf_model);
ADCS_returnState ADCS_get_ACP_loop_stat(uint16_t* time,
                                        uint8_t* execution_point);
ADCS_returnState ADCS_get_img_save_progress(uint8_t* percentage,
                                            uint8_t* status);

ADCS_returnState ADCS_get_measurements(adcs_measures* measurements);
ADCS_returnState ADCS_get_actuator(adcs_actuator* commands);
ADCS_returnState ADCS_get_estimation(adcs_estimate* data);
ADCS_returnState ADCS_get_ASGP4(bool* complete, uint8_t* err,
                                adcs_asgp4* asgp4);
void get_cam_sensor(cam_sensor* cam, uint8_t* address);
ADCS_returnState ADCS_get_raw_sensor(adcs_raw_sensor* measurements);
void get_ecef(ecef* coordinate, uint8_t* address);
ADCS_returnState ADCS_get_raw_GPS(adcs_raw_gps* measurements);
void get_star_data(star_data* coordinate, uint8_t* address, uint8_t i);
ADCS_returnState ADCS_get_star_tracker(adcs_star_track* measurements);
ADCS_returnState ADCS_get_MTM2_measurements(xyz16* Mag);
void get_current(float* measurement, uint8_t* address, float coef);
void get_temp(float* measurement, uint8_t* address, float coef);
ADCS_returnState ADCS_get_power_temp(adcs_pwr_temp* measurements);

#endif /* ADCS_HANDLER_H */
