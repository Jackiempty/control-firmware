/*
----------------------------------------------------------------------
File    : logger.c
Purpose : Source code for the data logger task. Data acquisition from
          multiple sensors will be done in this task.

          The log file is a binary file with the following format:
          [timestamp][sensor_id][data_length][data][EOL]
          where
            timestamp is a 32-bit unsigned integer,
            sensor_id is a 8-bit unsigned integer,
            data_length is a 8-bit unsigned integer,
            and followed by data_length bytes of data.
            EOL is a 16-bit unsigned integer 0x0D0A.

          Sensors IDs and data format are as follows:
            0x01 - LDPS, each LDPS data is 16-bit unsigned integer
              [N][LDPS_1][LDPS_2]...[LDPS_N]
            0x02 - Accelerometer, 3 axes raw data, each axis is 16-bit
              signed integer [X][Y][Z]
            0x03 - Gyroscope, 3 axes raw data, each axis is 16-bit
              signed integer [X][Y][Z]
            0x04 - Wheel speed, 4 wheels, each wheel is 32-bit float
              [W1][W2][W3][W4] (in RPM)

Revision: $Rev: 2023.49$
----------------------------------------------------------------------
*/

#include "logger.h"

#include "SEGGER_RTT.h"
#include "config.h"
#include "events.h"
#include "fx_api.h"
#include "stddef.h"
#include "usbd_cdc_if.h"

TX_THREAD logger_thread;

extern TX_EVENT_FLAGS_GROUP event_flags;

// Logger file objects
extern FX_MEDIA sdio_disk;
FX_FILE logger_file;

#if LDPS_ENABLE
// LDPS instance objects
#include "ldps.h"
extern ldps_t ldps[LDPS_N];
#endif

#if IMU_ENABLE
// IMU instance objects
#include "imu.h"
extern imu_t imu;
#endif

#if WHEEL_ENABLE
// Wheel instance objects
#include "wheel.h"
extern wheel_t wheel[WHEEL_N];
#endif

static inline void logger_output(char *buf, size_t len) {
#ifdef LOGGER_SD
  fx_file_write(&logger_file, buf, len);
#endif
#ifdef LOGGER_USB_SERIAL
  CDC_Transmit_FS((uint8_t *)buf, len);
#endif
  SEGGER_RTT_printf(0, "[LOGGER] 0x%02x\r\n", buf[4]);
}

void logger_thread_entry(ULONG thread_input) {
  UINT status = FX_SUCCESS;

  // Wait for the filesystem and config to be loaded
  ULONG recv_events_flags = 0;
  status = tx_event_flags_get(
      &event_flags, EVENT_BIT(EVENT_FS_INIT) | EVENT_BIT(EVENT_CONFIG_LOADED),
      TX_AND, &recv_events_flags, TX_WAIT_FOREVER);

  LOGGER_DEBUG("Logger thread started\n");

  int fid = 0;
  while (1) {
    char fn[128];
    sprintf(fn, "fsae-%04d.log", fid);
    status = fx_file_open(&sdio_disk, &logger_file, fn, FX_OPEN_FOR_WRITE);
    if (status != FX_SUCCESS) {
      fx_file_close(&logger_file);
      fx_file_create(&sdio_disk, fn);
      fx_file_open(&sdio_disk, &logger_file, fn, FX_OPEN_FOR_WRITE);
      fx_file_seek(&logger_file, 0);
      break;
    }
    fid++;
  }

  config_t *config = open_config_instance(0);

  // Start the logger
  while (1) {
    static char buf[128];
    uint32_t timestamp = tx_time_get();
    memcpy(buf, &timestamp, 4);

#if LDPS_ENABLE
    static uint32_t last_ldps_timestamp = 0;
    if (timestamp - last_ldps_timestamp > TX_TIMER_TICKS_PER_SECOND / 1000) {
      buf[4] = 0x01;
      buf[5] = LDPS_N * 2;

      for (size_t i = 0; i < LDPS_N; i++) {
        int16_t v = ldps_read(&ldps[i], &config->ldps_cal[i]);
        memcpy(buf + 6 + i * 2, &v, 2);
      }

      buf[6 + LDPS_N * 2] = 0x0D;
      buf[7 + LDPS_N * 2] = 0x0A;
      logger_output(buf, 8 + LDPS_N * 2);
      last_ldps_timestamp = timestamp;
    }
#endif

#if IMU_ENABLE
    static uint32_t last_acc_timestamp = 0;
    static uint32_t last_gyro_timestamp = 0;
    if (imu.acc.timestamp != last_acc_timestamp) {
      buf[4] = 0x02;
      buf[5] = 0x06;
      memcpy(buf + 6, &imu.acc_raw.x, 2);
      memcpy(buf + 8, &imu.acc_raw.y, 2);
      memcpy(buf + 10, &imu.acc_raw.z, 2);
      buf[12] = 0x0D;
      buf[13] = 0x0A;
      logger_output(buf, 14);
      last_acc_timestamp = imu.acc.timestamp;
    }

    if (imu.gyro.timestamp != last_gyro_timestamp) {
      buf[4] = 0x03;
      buf[5] = 0x06;
      memcpy(buf + 6, &imu.gyro_raw.x, 2);
      memcpy(buf + 8, &imu.gyro_raw.y, 2);
      memcpy(buf + 10, &imu.gyro_raw.z, 2);
      buf[12] = 0x0D;
      buf[13] = 0x0A;
      logger_output(buf, 14);
      last_gyro_timestamp = imu.gyro.timestamp;
    }
#endif

#if WHEEL_ENABLE
    static uint32_t last_wheel_timestamp = 0;
    if (timestamp - last_wheel_timestamp > TX_TIMER_TICKS_PER_SECOND / 1000) {
      buf[4] = 0x04;
      for (size_t i = 0; i < WHEEL_N; i++)
        memcpy(buf + 5 + i * 4, &wheel[i].rpm, 4);
      buf[WHEEL_N * 4 + 5] = 0x0D;
      buf[WHEEL_N * 4 + 6] = 0x0A;
      logger_output(buf, WHEEL_N * 4 + 7);
      last_wheel_timestamp = timestamp;
    }
#endif

#ifdef LOGGER_SD
    static uint32_t last_sd_timestamp = 0;
    if (timestamp - last_sd_timestamp > TX_TIMER_TICKS_PER_SECOND) {
      fx_media_flush(&sdio_disk);
      last_sd_timestamp = timestamp;
    }
#endif
  }
}
