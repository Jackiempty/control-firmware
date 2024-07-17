/*
----------------------------------------------------------------------
File    : adc.c
Purpose : User dirver functions for the linear displacement sernsor.
Revision: $Rev: 2023.43$
----------------------------------------------------------------------
*/

#include "adc.h"

#define MAX_INSTANCE 16
static adc_t instance_list[MAX_INSTANCE];

static inline float IIR_filter(float input, float prev, float alpha) {
  return alpha * input + (1 - alpha) * prev;
}

adc_t *open_adc_instance(uint32_t id) {
  if (id >= MAX_INSTANCE) return NULL;
  return &instance_list[id];
}

void adc_start() { adc_bsp_start(); }

void adc_set_buffer_pos(adc_t *adc, size_t pos) {
  adc_bsp_set_buffer_pos(adc, pos);
}

void adc_convert(adc_t *adc) {
  float last_value = adc->value;
  adc->value = ((float)((int32_t)*adc->buffer_ptr - (int32_t)adc->cal.offset)) *
               adc->cal.scale;
  adc->value = IIR_filter(adc->value, last_value, adc->alpha);
}

void adc_return_to_zero(adc_t *adc) { adc->cal.offset = *adc->buffer_ptr; }
