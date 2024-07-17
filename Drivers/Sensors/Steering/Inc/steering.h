#pragma once

#include <stdint.h>

typedef struct {
  int16_t angle;
  int8_t speed;
  uint32_t timestamp;
} steering_t;

steering_t *open_steering_instance(uint32_t id);
void steering_calibrate(steering_t *steering);

void steering_bsp_interrupt(steering_t *steering_t, void *arg1, void *arg2);
void steering_bsp_calibration();
