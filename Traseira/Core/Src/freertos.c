/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "rpm.h"
#include "error.h"
#include "can.h"
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
can_msg msg;



extern uint32_t rpm_itr[4];

extern CAN_HandleTypeDef hcan;
extern CAN_TxHeaderTypeDef txheader;
extern uint32_t txmailbox;



/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CAN_handler */
osThreadId_t CAN_handlerHandle;
const osThreadAttr_t CAN_handler_attributes = {
  .name = "CAN_handler",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for RPM_handler */
osThreadId_t RPM_handlerHandle;
const osThreadAttr_t RPM_handler_attributes = {
  .name = "RPM_handler",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for CAN_Q */
osMessageQueueId_t CAN_QHandle;
const osMessageQueueAttr_t CAN_Q_attributes = {
  .name = "CAN_Q"
};
/* Definitions for itr_events */
osEventFlagsId_t itr_eventsHandle;
const osEventFlagsAttr_t itr_events_attributes = {
  .name = "itr_events"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Start_CAN_handler(void *argument);
void Start_RPM_handler(void *argument);

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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of CAN_Q */
  CAN_QHandle = osMessageQueueNew (16, sizeof(uint16_t), &CAN_Q_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of CAN_handler */
  CAN_handlerHandle = osThreadNew(Start_CAN_handler, NULL, &CAN_handler_attributes);

  /* creation of RPM_handler */
  RPM_handlerHandle = osThreadNew(Start_RPM_handler, NULL, &RPM_handler_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of itr_events */
  itr_eventsHandle = osEventFlagsNew(&itr_events_attributes);

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

/* USER CODE BEGIN Header_Start_CAN_handler */
/**
* @brief Function implementing the CAN_handler thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_CAN_handler */
void Start_CAN_handler(void *argument)
{
  /* USER CODE BEGIN Start_CAN_handler */
  /* Infinite loop */
  for(;;)
  {
    if (osMessageQueueGetCount(CAN_QHandle) >= osMessageQueueGetCapacity(CAN_QHandle)) {
      osMessageQueueReset(CAN_QHandle);
      HAL_CAN_AddTxMessage(&hcan, &txheader, (uint8_t*) ERROR_CAN_QUEUE_FULL, &txmailbox);
    }

    if (osMessageQueueGet(CAN_QHandle, &msg, NULL, osWaitForever) == osOK) {
      
      
      HAL_CAN_AddTxMessage(&hcan, &txheader, (uint8_t*)msg.pdata, &txmailbox);
      free(msg.pdata);
    }

    

    osDelay(1);

    
  }
  /* USER CODE END Start_CAN_handler */
}

/* USER CODE BEGIN Header_Start_RPM_handler */
/**
* @brief Function implementing the RPM_handler thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_RPM_handler */
void Start_RPM_handler(void *argument)
{
  /* USER CODE BEGIN Start_RPM_handler */
  /* Infinite loop */
  for(;;)
  {
    osEventFlagsWait(itr_eventsHandle, ITR_RPM_FLAG, osFlagsWaitAny, osWaitForever);
    can_msg rpm_msg;
    rpm_msg.pdata = malloc(sizeof(float));
    rpm_msg.type = MSG_RPM;

    float rpm = rpm_calculate(rpm_itr);
    rpm_msg.pdata = &rpm;

    osMessageQueuePut(CAN_QHandle, &rpm_msg, NULL, 0);

    osEventFlagsClear(itr_eventsHandle, ITR_RPM_FLAG);
    
    osDelay(1);
  }
  /* USER CODE END Start_RPM_handler */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

