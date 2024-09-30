/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "msg.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LoRa_E22.h"
#include "virt_uart.h"
#include "msg.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern UART_HandleTypeDef huart7;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TxComms */
osThreadId_t TxCommsHandle;
const osThreadAttr_t TxComms_attributes = {
  .name = "TxComms",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for RxComms */
osThreadId_t RxCommsHandle;
const osThreadAttr_t RxComms_attributes = {
  .name = "RxComms",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for SysInit */
osThreadId_t SysInitHandle;
const osThreadAttr_t SysInit_attributes = {
  .name = "SysInit",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TxQueue */
osMessageQueueId_t TxQueueHandle;
const osMessageQueueAttr_t TxQueue_attributes = {
  .name = "TxQueue"
};
/* Definitions for SerialSmph */
osSemaphoreId_t SerialSmphHandle;
const osSemaphoreAttr_t SerialSmph_attributes = {
  .name = "SerialSmph"
};
/* Definitions for I2CSmph */
osSemaphoreId_t I2CSmphHandle;
const osSemaphoreAttr_t I2CSmph_attributes = {
  .name = "I2CSmph"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTxCommsTask(void *argument);
void StartRxCommsTask(void *argument);
void StartSysInitTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of SerialSmph */
  SerialSmphHandle = osSemaphoreNew(1, 1, &SerialSmph_attributes);

  /* creation of I2CSmph */
  I2CSmphHandle = osSemaphoreNew(1, 1, &I2CSmph_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of TxQueue */
  TxQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &TxQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of TxComms */
  TxCommsHandle = osThreadNew(StartTxCommsTask, NULL, &TxComms_attributes);

  /* creation of RxComms */
  RxCommsHandle = osThreadNew(StartRxCommsTask, NULL, &RxComms_attributes);

  /* creation of SysInit */
  SysInitHandle = osThreadNew(StartSysInitTask, NULL, &SysInit_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTxCommsTask */
/**
* @brief Function implementing the TxComms thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTxCommsTask */
void StartTxCommsTask(void *argument)
{
  /* USER CODE BEGIN StartTxCommsTask */
	E22 lora;
	E22_config lora_cfg;

	VIRT_UART_HandleTypeDef vuart;

	VIRT_UART_Init(&vuart);	//the max vuart message size is 512 bytes

	lora_init(&lora, &lora_cfg, GPIOF, GPIO_PIN_4, GPIO_PIN_6, GPIO_PIN_3, &huart7);
  /* Infinite loop */
  for(;;)
  {




    osDelay(1);

  }
  /* USER CODE END StartTxCommsTask */
}

/* USER CODE BEGIN Header_StartRxCommsTask */
/**
* @brief Function implementing the RxComms thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRxCommsTask */
void StartRxCommsTask(void *argument)
{
  /* USER CODE BEGIN StartRxCommsTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartRxCommsTask */
}

/* USER CODE BEGIN Header_StartSysInitTask */
/**
* @brief Function implementing the SysInit thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSysInitTask */
void StartSysInitTask(void *argument)
{
  /* USER CODE BEGIN StartSysInitTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartSysInitTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
