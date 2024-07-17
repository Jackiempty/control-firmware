/*
----------------------------------------------------------------------
File    : adc.h
Purpose : Implementation of the ADC driver for the Linear Displace-
          ment Sensor (LDPS) and Accelerator/Break pedal.
Revision: $Rev: 2024.12$
----------------------------------------------------------------------
*/

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "jsmn.h"

typedef struct {
  float scale;
  uint16_t offset;
} adc_cal_t;

typedef struct {
  uint16_t *buffer_ptr;
  float value;
  float alpha;
  adc_cal_t cal;
} adc_t;

void adc_start();

adc_t *open_adc_instance(uint32_t id);
void adc_set_buffer_pos(adc_t *adc, size_t pos);
void adc_convert(adc_t *adc);
void adc_return_to_zero(adc_t *adc);
int adc_config_hook(adc_t *instance, uint8_t *id, const char *json,
                    jsmntok_t *t);

void adc_bsp_start();
void adc_bsp_set_buffer_pos(adc_t *adc, size_t pos);
