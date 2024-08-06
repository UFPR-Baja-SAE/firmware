/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "msg.h"
#include "can.h"
/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */
#define ADC_TO_VOLTAGE 1241.2121212121212

typedef enum ADC_ARRAY_INDEX {
  ADC_BAT,
  ADC_FREIOT,
  ADC_FREIOD,
  ADC_COMB
} ADC_ARRAY_INDEX;

typedef struct adc_raw_values {
  uint16_t bat;
  uint16_t freioT;
  uint16_t freioD;
  uint16_t comb;
} adc_raw_values;

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */
void adc_read_values(adc_raw_values* raw);
void adc_convert_values(adc_raw_values* raw, float* conv);

void adc_create_msg(float* conv, can_msg* part1, can_msg* part2);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

