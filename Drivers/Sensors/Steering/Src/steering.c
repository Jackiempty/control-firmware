#include "steering.h"

#include <stddef.h>

#define MAX_INSTANCE 2
static steering_t instance_list[MAX_INSTANCE];

steering_t *open_steering_instance(uint32_t id) {
  if (id >= MAX_INSTANCE) return NULL;
  return &instance_list[id];
}

void steering_calibrate(steering_t *steering) { steering_bsp_calibration(); }
