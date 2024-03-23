/*
----------------------------------------------------------------------
File    : init.c
Purpose : Source file for the system initialization.
Revision: $Rev: 2023.49$
----------------------------------------------------------------------
*/

#include "init.h"

#include "adc.h"
#include "config.h"
#include "events.h"
#include "fx_api.h"
#include "imu.h"
#include "inverter.h"

TX_THREAD init_thread;

extern TX_EVENT_FLAGS_GROUP event_flags;

/* System Peripherals and Drivers Instances */

// Config json file objects
extern FX_MEDIA sdio_disk;
FX_FILE config_file;

void init_thread_entry(ULONG thread_input) {
  UINT status = FX_SUCCESS;

  // Wait for the filesystem to be loaded
  ULONG recv_events_flags = 0;
  status = tx_event_flags_get(&event_flags, EVENT_BIT(EVENT_FS_INIT), TX_AND,
                              &recv_events_flags, TX_WAIT_FOREVER);

  INIT_DEBUG("Init started\n");

  char buf[512];
  ULONG len;

  status =
      fx_file_open(&sdio_disk, &config_file, CONFIG_FILENAME, FX_OPEN_FOR_READ);
  if (status != FX_SUCCESS) {
    INIT_DEBUG("Failed to open config file\n");
    tx_thread_terminate(tx_thread_identify());
  }

  status = fx_file_read(&config_file, buf, sizeof(buf), &len);
  if (status != FX_SUCCESS) {
    INIT_DEBUG("Failed to read config file\n");
    tx_thread_terminate(tx_thread_identify());
  }

  status = fx_file_close(&config_file);
  if (status != FX_SUCCESS) {
    INIT_DEBUG("Failed to close config file\n");
    tx_thread_terminate(tx_thread_identify());
  }

#if ADC_ENABLE
  adc_t *apps_l = open_adc_instance(0);
  adc_t *apps_r = open_adc_instance(1);
  adc_t *bpps_l = open_adc_instance(2);
  adc_t *bpps_r = open_adc_instance(3);

  adc_start();

  adc_set_buffer_pos(apps_l, 0);
  adc_set_buffer_pos(apps_r, 1);
  adc_set_buffer_pos(bpps_l, 2);
  adc_set_buffer_pos(bpps_r, 3);
  adc_return_to_zero(apps_l);
  adc_return_to_zero(apps_r);
  adc_return_to_zero(bpps_l);
  adc_return_to_zero(bpps_r);

  uint8_t adc_id[4] = {0, 1, 2, 3};
  config_load(buf, len, apps_l, &adc_id[0], (config_hook_t)adc_config_hook,
              "adc");
  config_load(buf, len, apps_r, &adc_id[1], (config_hook_t)adc_config_hook,
              "adc");
  config_load(buf, len, bpps_l, &adc_id[2], (config_hook_t)adc_config_hook,
              "adc");
  config_load(buf, len, bpps_r, &adc_id[3], (config_hook_t)adc_config_hook,
              "adc");
#endif

#if IMU_ENABLE
  imu_t *imu = open_imu_instance(0);
  imu_set_type(imu, IMU_MTI600);
#endif

#if INVERTER_ENABLE
  inverter_t *inverter_R = open_inverter_instance(0);
  inverter_t *inverter_L = open_inverter_instance(1);
  inverter_R->type = INVERTER_PM100;
  inverter_R->hw_id = 0xA0;
  inverter_R->direction = 0x01;
  inverter_L->type = INVERTER_PM100;
  inverter_L->hw_id = 0x50;
  inverter_L->direction = 0x00;
  inverter_init(inverter_R);
  inverter_init(inverter_L);
#endif

  tx_event_flags_set(&event_flags, EVENT_BIT(EVENT_CONFIG_LOADED), TX_OR);

  INIT_DEBUG("Init finished\n");

  tx_thread_terminate(tx_thread_identify());
}
