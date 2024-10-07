#include "tempcvt.h"

uint16_t temp_read() {
  uint16_t raw;
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, RESET); //maybe don't hard code the pin lmao, i can't be bothered though so fuck it
  if (HAL_SPI_Receive(&hspi2, &raw, sizeof(uint16_t), 1) != HAL_OK) {
    return -1;
  }
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, SET);
  return raw;
}

uint32_t temp_convert(uint16_t raw) {
  float convert;
  uint16_t tmp;

  tmp = raw & 0b01111111111000;
  tmp = tmp >> 3;

  convert = TEMP_RAW_TO_REAL * tmp;
  
  return (uint32_t) (convert * 1000);
}
