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
#include "can.h"
#include "tempcvt.h"
#include "adc.h"
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




extern uint32_t rpm_itr[RPM_SAMPLES];

extern CAN_HandleTypeDef hcan;
extern CAN_TxHeaderTypeDef txheader;
extern uint32_t txmailbox;

extern CAN_RxHeaderTypeDef rxheader;
extern uint8_t* rxdata;

extern uint32_t polling_delay, itr_delay, comms_delay;

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
/* Definitions for Itr_handler */
osThreadId_t Itr_handlerHandle;
const osThreadAttr_t Itr_handler_attributes = {
  .name = "Itr_handler",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime1,
};
/* Definitions for Polling_handler */
osThreadId_t Polling_handlerHandle;
const osThreadAttr_t Polling_handler_attributes = {
  .name = "Polling_handler",
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
void Start_Itr_handler(void *argument);
void Start_Polling_handler(void *argument);

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
  CAN_QHandle = osMessageQueueNew (8, sizeof(can_msg), &CAN_Q_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of CAN_handler */
  CAN_handlerHandle = osThreadNew(Start_CAN_handler, NULL, &CAN_handler_attributes);

  /* creation of Itr_handler */
  Itr_handlerHandle = osThreadNew(Start_Itr_handler, NULL, &Itr_handler_attributes);

  /* creation of Polling_handler */
  Polling_handlerHandle = osThreadNew(Start_Polling_handler, NULL, &Polling_handler_attributes);

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

/*
todo: see if mail queues are better than regular queues for this (what we are doing is basically a scuffed mail queue anyway)

actually switch to mail queues since message queues used like this can create memory leaks very easily
if a task generates a new message faster than the can_handler task can consume it's previous message,
the pointer to the allocated data (pdata) will be changed and the previous will never be freed

this is fucking aids as shit yikes

this will leak anyway, use semaphores so that tasks can't malloc on top of the already allocated memory
*/

void Start_CAN_handler(void *argument)
{
  /* USER CODE BEGIN Start_CAN_handler */
  can_msg *msg;
  msg = malloc(sizeof(can_msg));
  /* Infinite loop */
  for(;;)
  {
    if (osMessageQueueGetCount(CAN_QHandle) >= osMessageQueueGetCapacity(CAN_QHandle)) {
      osMessageQueueReset(CAN_QHandle);
      ERROR_MSG err = ERROR_CAN_QUEUE_FULL;
      can_setup_message(msg, MSG_ERROR, &err, 1);
      can_send_message(msg);
    }

    if (osMessageQueueGet(CAN_QHandle, &msg, NULL, osWaitForever) == osOK) {
      can_send_message(msg);
      free(msg->pdata);
    }

    /*
    todo: find a way of not using global variables for this (maybe have 2 different mutexes or whatever)
    */

    if (osThreadFlagsGet() == SIGNAL_CAN_RX) {
      //Handle the message
      switch (rxheader.StdId) {
        case FREQ_ITR:
          itr_delay = (uint32_t)* rxdata;
          break;
        case FREQ_POLLING:
          polling_delay = (uint32_t)* rxdata;
          break;
        case FREQ_COMMS:
          comms_delay = (uint32_t)* rxdata;
      }
    }
    osDelay(comms_delay);
  /* USER CODE END Start_CAN_handler */
  }
}

/* USER CODE BEGIN Header_Start_Itr_handler */
/**
* @brief Function implementing the Itr_handler thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Itr_handler */
void Start_Itr_handler(void *argument)
{
  /* USER CODE BEGIN Start_Itr_handler */
  can_msg *rpm_msg;
  rpm_msg = malloc(sizeof(can_msg));
  /* Infinite loop */
  for(;;)
  {
    osEventFlagsWait(itr_eventsHandle, ITR_RPM_FLAG, osFlagsWaitAny, osWaitForever);

    float rpm = rpm_calculate(rpm_itr);
    
    can_setup_message(&rpm_msg, MSG_RPM, &rpm, sizeof(float));
    osMessageQueuePut(CAN_QHandle, &rpm_msg, NULL, 0);

    osEventFlagsClear(itr_eventsHandle, ITR_RPM_FLAG);

    osDelay(itr_delay);

  }
  /* USER CODE END Start_Itr_handler */
}

/* USER CODE BEGIN Header_Start_Polling_handler */
/**
* @brief Function implementing the Polling_handler thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Polling_handler */
void Start_Polling_handler(void *argument)
{
  /* USER CODE BEGIN Start_Polling_handler */
  float values[4];
  adc_raw_values raw_vals;
  can_msg* p1;
  can_msg* p2;
  can_msg* temp;
  uint16_t raw_temp_cvt;
  float temp_cvt;
  temp = malloc(sizeof(can_msg));
  p1 = malloc(sizeof(can_msg));
  p2 = malloc(sizeof(can_msg));
  /* Infinite loop */
  for(;;)
  {
    adc_read_values(&raw_vals);
    adc_convert_values(&raw_vals, values);

    adc_create_msg(values, p1, p2);

    osMessageQueuePut(CAN_QHandle, p1, NULL, 0);
    osMessageQueuePut(CAN_QHandle, p2, NULL, 0);

    raw_temp_cvt = temp_read();
    if (raw_temp_cvt <= 0) {
      //todo: error handling
    } 
    else {
      temp_cvt = temp_convert(raw_temp_cvt);
      can_setup_message(temp, MSG_TEMPERATURE, &temp_cvt, sizeof(float));
      osMessageQueuePut(CAN_QHandle, temp, NULL, 0);
    }
    osDelay(polling_delay);
    
  }
  /* USER CODE END Start_Polling_handler */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

