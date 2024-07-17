#include <stdint.h>

#include "SEGGER_RTT.h"
#include "steering.h"

#ifdef USE_HAL_DRIVER

#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#endif

void steering_bsp_interrupt(steering_t *steering, void *arg1, void *arg2) {
  CAN_RxHeaderTypeDef *rx_header = (CAN_RxHeaderTypeDef *)arg1;
  uint8_t *rx_data = (uint8_t *)arg2;
  switch (rx_header->StdId) {
    case 0x2B0: {
      uint16_t __angle = (rx_data[1] << 8) | rx_data[0];
      uint8_t __speed = rx_data[2];
      steering->angle = *(int16_t *)&__angle;
      steering->speed = *(int8_t *)&__speed;
      steering->timestamp++;
      break;
    }
    default:
      break;
  }
}

void steering_bsp_calibration() {
  extern CAN_HandleTypeDef hcan1;
  CAN_TxHeaderTypeDef tx_handler = {
      .StdId = 0x7C0,
      .ExtId = 0x01,
      .RTR = CAN_RTR_DATA,
      .IDE = CAN_ID_STD,
      .DLC = 2,
      .TransmitGlobalTime = DISABLE,
  };
  uint8_t tx_data[2] = {0x03, 0x00};
  HAL_CAN_AddTxMessage(&hcan1, &tx_handler, tx_data, NULL);
}
#endif
