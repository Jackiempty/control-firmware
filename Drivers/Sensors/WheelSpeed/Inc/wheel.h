/*
----------------------------------------------------------------------
File    : wheel.h
Purpose : Implementation of the bosch 2-wire wheel speed sensors.
Revision: $Rev: 2024.05$
----------------------------------------------------------------------
*/

#ifndef WHEEL_H
#define WHEEL_H

#include <stddef.h>
#include <stdint.h>

#include "utils.h"

#define STOP_SPEED 0.1f
#define MS_PRE_TICK 0.1f

#define TO_RPM(ticks_pre_rev, delta_t) \
  (60000.0 / (ticks_pre_rev) / (delta_t) / MS_PRE_TICK)

typedef struct {
  void *handler;
  uint8_t channel;
} wheel_hw_t;

typedef struct {
  wheel_hw_t hw;
  float rpm;
  uint16_t last_tick;
  uint16_t tick;
  uint8_t ticks_pre_rev;
  uint8_t invalid;
} wheel_t;

wheel_t *open_wheel_instance(uint32_t id);
void wheel_init(wheel_t *w);
void wheel_update(wheel_t *w);

void wheel_bsp_init(wheel_t *w);
void wheel_bsp_interrupt(wheel_t *w, void *arg);
void wheel_bsp_overflow_interrupt(wheel_t *w, void *arg);

#endif
