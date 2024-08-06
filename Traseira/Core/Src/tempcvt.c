#include "tempcvt.h"

uint16_t temp_read() {
  uint16_t raw;
  uint8_t* pdata = (uint8_t*)&raw;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, RESET);
  if (HAL_SPI_Receive(&hspi2, pdata, sizeof(uint16_t), 1) != HAL_OK) {
    return -1;
  }
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, SET);
  return raw;
}

float temp_convert(uint16_t raw) {
  float convert;
  uint16_t tmp;

  tmp = raw & 0b01111111111000;
  tmp = tmp >> 3;

  convert = TEMP_RAW_TO_REAL * tmp;
  
  return convert;
}
