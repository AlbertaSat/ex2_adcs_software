#ifndef ADCS_H
#define ADCS_H

#include "adcs_handler.h"

typedef struct __attribute__((packed))
{
    int16_t Estimated_Angular_Rate;
    int16_t Estimated_Angular_Angle;
    int16_t Sat_Position_ECI;
    int16_t Sat_Velocity_ECI;
    int16_t Sat_Position_LLH;
    int16_t ECEF_Position;
    int16_t Coarse_Sun_Vector;
    int16_t Fine_Sun_Vector;
    int16_t Nadir_Vector;
    int16_t Wheel_Speed;
    int16_t Mag_Field_Vector;
    bool Comm_Status;
    uint16_t Wheel_Currents;
    uint16_t CubeSense_Current;
    uint16_t CubeControl_Current;
    uint16_t ADCS_Misc_Current;
    int16_t ADCS_Temp;
    int16_t Rate_Sensor_Temp;
    bool ADCS_State;
} ADCS_HouseKeeping;


// Common Telecommands
ADCS_returnState HAL_ADCS_reset();
ADCS_returnState HAL_ADCS_reset_log_pointer();
ADCS_returnState HAL_ADCS_advance_log_pointer();
ADCS_returnState HAL_ADCS_reset_boot_registers();
ADCS_returnState HAL_ADCS_format_sd_card();
ADCS_returnState HAL_ADCS_erase_file(uint8_t file_type, uint8_t file_counter, bool erase_all);
ADCS_returnState HAL_ADCS_load_file_download_block(uint8_t file_type, uint8_t counter, uint32_t offset, uint16_t block_length);
ADCS_returnState HAL_ADCS_advance_file_list_read_pointer();
ADCS_returnState HAL_ADCS_initiate_file_upload(uint8_t file_dest, uint8_t block_size);
ADCS_returnState HAL_ADCS_file_upload_packet(uint16_t packet_number, char* file_bytes);
ADCS_returnState HAL_ADCS_finalize_upload_block(uint8_t file_dest, uint32_t offset, uint16_t block_length);
ADCS_returnState HAL_ADCS_reset_upload_block();
ADCS_returnState HAL_ADCS_reset_file_list_read_pointer();
ADCS_returnState HAL_ADCS_initiate_download_burst(uint8_t msg_length, bool ignore_hole_map);

// Common Telemetry
ADCS_returnState HAL_ADCS_get_node_identification(
    uint8_t* node_type, uint8_t* interface_ver, uint8_t* major_firm_ver,
    uint8_t* minor_firm_ver, uint16_t* runtime_s, uint16_t* runtime_ms);
ADCS_returnState HAL_ADCS_get_boot_program_stat(uint8_t* mcu_reset_cause,
                                            uint8_t* boot_cause,
                                            uint16_t* boot_count,
                                            uint8_t* boot_idx);
ADCS_returnState HAL_ADCS_get_boot_index(uint8_t* program_idx, uint8_t* boot_stat);
ADCS_returnState HAL_ADCS_get_last_logged_event(uint32_t* time, uint8_t* event_id,
                                            uint8_t* event_param);
ADCS_returnState HAL_ADCS_get_SD_format_progress(bool* format_busy,
                                             bool* erase_all_busy);
ADCS_returnState HAL_ADCS_get_TC_ack(uint8_t* last_tc_id, bool* tc_processed,
                                 ADCS_returnState* tc_err_stat,
                                 uint8_t* tc_err_idx);
ADCS_returnState HAL_ADCS_get_file_download_buffer(uint16_t* packet_count,
                                               uint8_t* file[20]);
ADCS_returnState HAL_ADCS_get_file_download_block_stat(bool* ready, bool* param_err,
                                                   uint16_t* crc16_checksum,
                                                   uint16_t* length);
ADCS_returnState HAL_ADCS_get_file_info(uint8_t* type, bool* updating,
                                    uint8_t* counter, uint32_t* size,
                                    uint32_t* time, uint16_t* crc16_checksum);
ADCS_returnState HAL_ADCS_get_init_upload_stat(bool* busy);
ADCS_returnState HAL_ADCS_get_finalize_upload_stat(bool* busy, bool* err);
ADCS_returnState HAL_ADCS_get_upload_crc16_checksum(uint16_t* checksum);
ADCS_returnState HAL_ADCS_get_SRAM_latchup_count(uint16_t* sram1, uint16_t* sram2);
ADCS_returnState HAL_ADCS_get_EDAC_err_count(uint16_t* single_sram,
                                         uint16_t* double_sram,
                                         uint16_t* multi_sram);
ADCS_returnState HAL_ADCS_get_comms_stat(uint16_t* TC_num, uint16_t* TM_num,
                                     uint8_t* flags_arr);

// Common Config Msgs
ADCS_returnState HAL_ADCS_set_cache_en_state(bool en_state);
ADCS_returnState HAL_ADCS_set_sram_scrub_size(uint16_t size);
ADCS_returnState HAL_ADCS_set_UnixTime_save_config(uint8_t when, uint8_t period);
ADCS_returnState HAL_ADCS_set_hole_map(uint8_t* hole_map, uint8_t num);
ADCS_returnState HAL_ADCS_set_unix_t(uint32_t unix_t, uint16_t count_ms);
ADCS_returnState HAL_ADCS_get_cache_en_state(bool* en_state);
ADCS_returnState HAL_ADCS_get_sram_scrub_size(uint16_t* size);
ADCS_returnState HAL_ADCS_get_UnixTime_save_config(uint8_t* when, uint8_t* period);
ADCS_returnState HAL_ADCS_get_hole_map(uint8_t* hole_map, uint8_t num);
ADCS_returnState HAL_ADCS_get_unix_t(uint32_t* unix_t, uint16_t* count_ms);

// BootLoader Telecommands
ADCS_returnState HAL_ADCS_clear_err_flags();
ADCS_returnState HAL_ADCS_set_boot_index(uint8_t index);
ADCS_returnState HAL_ADCS_run_selected_program();
ADCS_returnState HAL_ADCS_read_program_info(uint8_t index);
ADCS_returnState HAL_ADCS_copy_program_internal_flash(uint8_t index, uint8_t overwrite_flag);

// BootLoader Telemetries
ADCS_returnState HAL_ADCS_get_bootloader_state(uint16_t* uptime,
                                           uint8_t* flags_arr);
ADCS_returnState HAL_ADCS_get_program_info(uint8_t* index, bool* busy,
                                       uint32_t* file_size,
                                       uint16_t* crc16_checksum);
ADCS_returnState HAL_ADCS_copy_internal_flash_progress(bool* busy, bool* err);

// ACP Telecommands
ADCS_returnState HAL_ADCS_deploy_magnetometer_boom(uint8_t actuation_timeout);
ADCS_returnState HAL_ADCS_set_enabled_state(uint8_t state);
ADCS_returnState HAL_ADCS_clear_latched_errs(bool adcs_flag, bool hk_flag);
ADCS_returnState HAL_ADCS_set_attitude_ctrl_mode(uint8_t ctrl_mode,
                                             uint16_t timeout);
ADCS_returnState HAL_ADCS_set_attitude_estimate_mode(uint8_t mode);
ADCS_returnState HAL_ADCS_trigger_adcs_loop();
ADCS_returnState HAL_ADCS_trigger_adcs_loop_sim(sim_sensor_data sim_data);
ADCS_returnState HAL_ADCS_set_ASGP4_rune_mode(uint8_t mode);
ADCS_returnState HAL_ADCS_trigger_ASGP4();
ADCS_returnState HAL_ADCS_set_MTM_op_mode(uint8_t mode);
ADCS_returnState HAL_ADCS_cnv2jpg(uint8_t source, uint8_t QF,
                              uint8_t white_balance);
ADCS_returnState HAL_ADCS_save_img(uint8_t camera, uint8_t img_size);
ADCS_returnState HAL_ADCS_set_magnetorquer_output(xyz16 duty_cycle);
ADCS_returnState HAL_ADCS_set_wheel_speed(xyz16 speed);
ADCS_returnState HAL_ADCS_save_config();
ADCS_returnState HAL_ADCS_save_orbit_params();

// ACP Telemetry
ADCS_returnState HAL_ADCS_get_current_state(adcs_state* data);

ADCS_returnState HAL_ADCS_get_jpg_cnv_progress(uint8_t* percentage, uint8_t* result,
                                           uint8_t* file_counter);
ADCS_returnState HAL_ADCS_get_cubeACP_state(uint8_t* flags_arr);
ADCS_returnState HAL_ADCS_get_execution_times(uint16_t* adcs_update,
                                          uint16_t* sensor_comms,
                                          uint16_t* sgp4_propag,
                                          uint16_t* igrf_model);
ADCS_returnState HAL_ADCS_get_ACP_loop_stat(uint16_t* time,
                                        uint8_t* execution_point);
ADCS_returnState HAL_ADCS_get_img_save_progress(uint8_t* percentage,
                                            uint8_t* status);

ADCS_returnState HAL_ADCS_get_measurements(adcs_measures* measurements);
ADCS_returnState HAL_ADCS_get_actuator(adcs_actuator* commands);
ADCS_returnState HAL_ADCS_get_estimation(adcs_estimate* data);
ADCS_returnState HAL_ADCS_get_ASGP4(bool* complete, uint8_t* err,
                                adcs_asgp4* asgp4);
ADCS_returnState HAL_ADCS_get_raw_sensor(adcs_raw_sensor* measurements);
ADCS_returnState HAL_ADCS_get_raw_GPS(adcs_raw_gps* measurements);
ADCS_returnState HAL_ADCS_get_star_tracker(adcs_star_track* measurements);
ADCS_returnState HAL_ADCS_get_MTM2_measurements(xyz16* Mag);
ADCS_returnState HAL_ADCS_get_power_temp(adcs_pwr_temp* measurements);

// ACP Config Msgs
ADCS_returnState HAL_ADCS_set_power_control(uint8_t* control);
ADCS_returnState HAL_ADCS_get_power_control(uint8_t* control);
ADCS_returnState HAL_ADCS_set_attitude_angle(xyz att_angle);
ADCS_returnState HAL_ADCS_get_attitude_angle(xyz* att_angle);
ADCS_returnState HAL_ADCS_set_track_controller(xyz target);
ADCS_returnState HAL_ADCS_get_track_controller(xyz* target);
ADCS_returnState HAL_ADCS_set_log_config(uint8_t* flags_arr, uint16_t period,
                                     uint8_t dest, uint8_t log);
ADCS_returnState HAL_ADCS_get_log_config(uint8_t* flags_arr, uint16_t* period,
                                     uint8_t* dest, uint8_t log);
ADCS_returnState HAL_ADCS_set_inertial_ref(xyz iner_ref);
ADCS_returnState HAL_ADCS_get_inertial_ref(xyz* iner_ref);

ADCS_returnState HAL_ADCS_set_sgp4_orbit_params(adcs_sgp4 params);
ADCS_returnState HAL_ADCS_get_sgp4_orbit_params(adcs_sgp4* params);
ADCS_returnState HAL_ADCS_set_system_config(adcs_sysConfig config);
ADCS_returnState HAL_ADCS_get_system_config(adcs_sysConfig* config);
ADCS_returnState HAL_ADCS_set_MTQ_config(xyzu8 params);
ADCS_returnState HAL_ADCS_set_RW_config(uint8_t* RW);
ADCS_returnState HAL_ADCS_set_rate_gyro(rate_gyro_config params);
ADCS_returnState HAL_ADCS_set_css_config(css_config config);
ADCS_returnState HAL_ADCS_set_star_track_config(cubestar_config config);
ADCS_returnState HAL_ADCS_set_cubesense_config(cubesense_config params);
ADCS_returnState HAL_ADCS_set_mtm_config(mtm_config params, uint8_t mtm);
ADCS_returnState HAL_ADCS_set_detumble_config(detumble_config config);
ADCS_returnState HAL_ADCS_set_ywheel_config(ywheel_ctrl_config params);
ADCS_returnState HAL_ADCS_set_rwheel_config(rwheel_ctrl_config params);
ADCS_returnState HAL_ADCS_set_tracking_config(track_ctrl_config params);
ADCS_returnState HAL_ADCS_set_MoI_mat(moment_inertia_config cell);
ADCS_returnState HAL_ADCS_set_estimation_config(estimation_config config);
ADCS_returnState HAL_ADCS_set_usercoded_setting(usercoded_setting setting);
ADCS_returnState HAL_ADCS_set_asgp4_setting(aspg4_setting setting);
ADCS_returnState HAL_ADCS_get_full_config(adcs_config* config);

ADCS_returnState HAL_ADCS_getHK(ADCS_HouseKeeping* adcs_hk);

#endif /* ADCS_HAL_H */