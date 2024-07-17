/*
----------------------------------------------------------------------
File    : adc_bsp.c
Purpose : BSP(Board Support Package) for the linear displacement
          sernsor. Curently only implemented for STM32F407xx via HAL.
Revision: $Rev: 2024.12$
----------------------------------------------------------------------
*/

#include "adc.h"

#ifdef USE_HAL_DRIVER

#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#endif

#define DMA_CHANNEL 4

extern ADC_HandleTypeDef hadc1;

static uint16_t dma_buffer[DMA_CHANNEL];

void adc_bsp_start() {
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)dma_buffer, DMA_CHANNEL) !=
      HAL_OK) {
    while (1)
      ;
  }
  HAL_Delay(500);
}

void adc_bsp_set_buffer_pos(adc_t *adc, size_t pos) {
  adc->buffer_ptr = &dma_buffer[pos];
}

#endif
