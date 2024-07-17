#include "wheel.h"

#define MAX_INSTANCE 4

static wheel_t wheel_instance[MAX_INSTANCE];

wheel_t *open_wheel_instance(uint32_t id) {
  if (id >= MAX_INSTANCE) return NULL;
  return &wheel_instance[id];
}

void wheel_init(wheel_t *w) {
  if (!w) return;
  w->invalid = 1;
  wheel_bsp_init(w);
}

void wheel_update(wheel_t *w) {
  if (!w) return;
  uint16_t delta_t = INTERVAL_16U(w->tick, w->last_tick);
  w->rpm = TO_RPM(w->ticks_pre_rev, delta_t);
}
