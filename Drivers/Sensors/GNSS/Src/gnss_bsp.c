#include "gnss.h"

#ifdef USE_HAL_DRIVER

#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#endif

void gnss_bsp_init(gnss_t *instance) {
  HAL_UARTEx_ReceiveToIdle_DMA(instance->hw.handler, instance->hw.buffer,
                               sizeof(instance->hw.buffer));
}

void gnss_bsp_interrupt(gnss_t *instance, uint8_t *data, size_t len) {
  HAL_UARTEx_ReceiveToIdle_DMA(instance->hw.handler, instance->hw.buffer,
                               sizeof(instance->hw.buffer));
  gnss_process(instance, data, len);
}
#endif
