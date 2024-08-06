/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.h
  * @brief   This file contains all the function prototypes for
  *          the can.c file
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
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "msg.h"
#include "signals.h"
/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan;

/* USER CODE BEGIN Private defines */
typedef struct can_msg {
  MSG_TYPES type;
  uint8_t size;
  void* pdata;
} can_msg;
/* USER CODE END Private defines */

void MX_CAN_Init(void);

/* USER CODE BEGIN Prototypes */
void can_setup_message(can_msg* pmsg, MSG_TYPES type, const void* pdata, uint16_t size);

void can_send_message(const can_msg* pmsg);

/*
what this function should be able to do:
  set diferent data aqquisition frequencies
  make the stm go to sleep (not sure if possible)
  wake up the stm (not sure if possible)
  turn off each sensor
  turn on each sensor

this is probably the most important function in here
having this be very versatile would be amazing for tests and simply not obliterating our storage in the front
*/
void can_handle_rx_msg();

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */