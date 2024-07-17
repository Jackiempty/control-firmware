#include "wheel.h"

#ifdef USE_HAL_DRIVER

#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#endif

#define US_PRE_TICK 1UL

#define WHEEL_TIM_CHANNEL_INT(CHANNEL)   \
  ((CHANNEL) == TIM_CHANNEL_1            \
       ? 1                               \
       : ((CHANNEL) == TIM_CHANNEL_2 ? 2 \
                                     : ((CHANNEL) == TIM_CHANNEL_3 ? 3 : 4)))

void wheel_bsp_init(wheel_t *w) {
  HAL_TIM_IC_Start_IT(w->hw.handler, w->hw.channel);
  HAL_TIM_Base_Start_IT(w->hw.handler);
}

void wheel_bsp_interrupt(wheel_t *w, void *arg) {
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)arg;
  TIM_HandleTypeDef *handler = w->hw.handler;
  if (htim->Instance == handler->Instance &&
      htim->Channel == (1 << (WHEEL_TIM_CHANNEL_INT(w->hw.channel) - 1))) {
    w->tick = HAL_TIM_ReadCapturedValue(htim, w->hw.channel);
    wheel_update(w->invalid ? NULL : w);
    w->invalid = w->invalid ? 0 : w->rpm <= STOP_SPEED;
    w->last_tick = w->tick;
    w->rpm = w->invalid ? 0 : w->rpm;
  }
}

void wheel_bsp_overflow_interrupt(wheel_t *w, void *arg) {
  TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)arg;
  TIM_HandleTypeDef *handler = w->hw.handler;
  if (htim->Instance == handler->Instance &&
      htim->Channel == (1 << (WHEEL_TIM_CHANNEL_INT(w->hw.channel) - 1))) {
    uint16_t delta_t = INTERVAL_16U(
        HAL_TIM_ReadCapturedValue(htim, w->hw.channel), w->last_tick);
    if (TO_RPM(w->ticks_pre_rev, delta_t) <= STOP_SPEED || w->invalid > 0) {
      w->invalid = 1;
      w->rpm = 0;
    }
  }
}

#endif
