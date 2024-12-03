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
extern uint16_t frequency[5];
extern uint8_t unit_state[5];

extern uint32_t rpm_itr[RPM_SAMPLES];

extern CAN_HandleTypeDef hcan;
extern CAN_TxHeaderTypeDef txheader;
extern uint32_t txmailbox;

extern CAN_RxHeaderTypeDef rxheader;
extern uint8_t* rxdata;

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
  .priority = (osPriority_t) osPriorityRealtime,
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
  .priority = (osPriority_t) osPriorityNormal,
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
  CAN_QHandle = osMessageQueueNew (8, sizeof(msg_all), &CAN_Q_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */

  Polling_handlerHandle = osThreadNew(Start_Polling_handler, NULL, &Polling_handler_attributes);
  /* creation of defaultTask */
  

  /* creation of CAN_handler */
  CAN_handlerHandle = osThreadNew(Start_CAN_handler, NULL, &CAN_handler_attributes);

  /* creation of Itr_handler */
  Itr_handlerHandle = osThreadNew(Start_Itr_handler, NULL, &Itr_handler_attributes);

  /* creation of Polling_handler */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

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
    osThreadFlagsWait(0XFFFF, osFlagsWaitAny, osWaitForever);

    //kill threads where all units are stopped, create threads where a unit has been restarted
    osThreadState_t tstate = osThreadGetState(Itr_handlerHandle);
    if (osThreadFlagsGet() == CONTROL_FLAG) {
      if ((unit_state[CONTROL_RPM] | unit_state[CONTROL_VELOCITY]) == 0) {
        if (tstate == 4)break;
        else osThreadTerminate(Itr_handlerHandle);
      } 
      
      else if (tstate == 4) {
        osThreadNew(Start_Itr_handler, NULL, &Itr_handler_attributes);
      }

      tstate = osThreadGetState(Polling_handlerHandle);
      if ((unit_state[CONTROL_FUEL] | unit_state[CONTROL_TEMPERATURE]) == 0) {
        if (tstate == 4) break;
        else osThreadTerminate(Polling_handlerHandle);
      }

      else if (tstate == 4) {
        osThreadNew(Start_Polling_handler, NULL, &Polling_handler_attributes);
      }
    }
    osDelay(10);
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
  msg_all msg;

  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, RESET);
    if (osMessageQueueGetCount(CAN_QHandle) >= osMessageQueueGetCapacity(CAN_QHandle)) {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, SET);

      osMessageQueueReset(CAN_QHandle);
      msg_error err;
      err.code = ERROR_CAN_QUEUE_FULL;
      err.timestamp = osKernelGetTickCount();
      msg_create_generic(&msg, MSG_ERROR, sizeof(msg_error), &err);
      can_send_message(&msg);
    }

    if (osMessageQueueGet(CAN_QHandle, &msg, NULL, osWaitForever) == osOK) {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, SET);
      can_send_message(&msg);
      free(msg.pdata);
      osDelay(1);
    }

    /*
    todo: find a way of not using global variables for this (maybe have 2 different mutexes or whatever)
    probably best to just use global variables and fuck it
    */

    if (osThreadFlagsGet() == SIGNAL_CAN_RX) {
      //Handle the message
      can_handle_rx_msg();
    }
    osDelay(frequency[CONTROL_CAN]);
  }
  /* USER CODE END Start_CAN_handler */
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
  msg_all msg;
  msg_rpm data;
  msg_warning warn;
  msg_error err;

  uint32_t last_rpm = 0;
  uint32_t last_vel = 0;

  uint8_t strikes_rpm = 0;
  uint8_t strikes_vel = 0;
  
  /* Infinite loop */
  for(;;)
  {
    osEventFlagsWait(itr_eventsHandle, 0xFFFF, osFlagsWaitAny, osWaitForever);

    if (((osEventFlagsGet(itr_eventsHandle) & ITR_RPM_FLAG) == ITR_RPM_FLAG) && unit_state[CONTROL_RPM] == 1) {
      if (last_rpm + frequency[CONTROL_RPM] < osKernelGetTickCount()) {
        uint32_t ts = osKernelGetTickCount();
        data.rpm = rpm_calculate(rpm_itr);
        if (data.rpm == -1) {
          strikes_rpm++;

          warn.code = WARNING_RPM_DEBOUNCE;
          warn.timestamp = ts;

          msg_create_generic(&msg, sizeof(msg_warning), MSG_WARNING, &warn);
          osMessageQueuePut(CAN_QHandle, &msg, NULL, 0);

          free(msg.pdata);
          if (strikes_rpm >= 3) {
            strikes_rpm = 0;
            
            err.code = ERROR_FAULTY_RPM;
            err.timestamp = osKernelGetTickCount();

            msg_create_generic(&msg, sizeof(msg_error), MSG_ERROR, &err);
            osMessageQueuePut(CAN_QHandle, &msg, NULL, 0);

            free(msg.pdata);
          }
          break;
        }
        data.timestamp = ts;
    
        msg_create_generic(&msg, MSG_RPM, sizeof(float), &data);
        osMessageQueuePut(CAN_QHandle, &msg, NULL, 0);
    
        osEventFlagsClear(itr_eventsHandle, ITR_RPM_FLAG);
        last_rpm = osKernelGetTickCount();
      }
    }

    if (((osEventFlagsGet(itr_eventsHandle) & ITR_VEL_FLAG) == ITR_VEL_FLAG) 
          && unit_state[CONTROL_VELOCITY]) {
      if (last_vel + frequency[CONTROL_VELOCITY] < osKernelGetTickCount()) {
      //TODO: speed calculations
      //there probably isn't going to be any speed data on the car, but it shouldn't be hard to do 
      //if we manage to get it working
      //plus do all the error handling, same as rpm
      osEventFlagsClear(itr_eventsHandle, ITR_VEL_FLAG);
      last_vel = osKernelGetTickCount();
      }
    }
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

  msg_all msg;
  msg_tempcvt cvt;
  msg_fuel fuel;

  msg_error err;
  msg_warning warn;

  uint8_t strike_fuel;
  uint8_t strike_temp;

  uint16_t raw_temp_cvt;

  uint32_t last_temp = 0;
  uint32_t last_fuel = 0;
  /* Infinite loop */
  for(;;)
  {
    if ((last_temp + frequency[CONTROL_TEMPERATURE] < osKernelGetTickCount()) 
        && unit_state[CONTROL_TEMPERATURE] == 1) {
      uint8_t is_warn = 0;
      uint8_t is_err = 0;
      uint32_t ts = osKernelGetTickCount();
      if (strike_fuel >= 3) {
        is_err = 1;
        err.code = ERROR_FAULTY_TEMP;
        err.timestamp = ts;
      }
      
      raw_temp_cvt = temp_read();

      if (raw_temp_cvt == -1) {
        strike_temp++;
        is_warn = 1;

        warn.code = WARNING_TEMP_SPI;
        warn.timestamp = ts;
      } else {
        cvt.temp = temp_convert(raw_temp_cvt);

        if (cvt.temp < 10000) {
          strike_temp++;
          is_warn = 1;

          warn.code = WARNING_TEMP_FAULTY;
          warn.timestamp = ts;
        } else
        if (cvt.temp > 100000) {
          is_warn = 1;
          warn.timestamp = ts;
          if (cvt.temp > 150000) {
            strike_temp++;
          
            warn.code = WARNING_TEMP_FAULTY;
          } else {
            warn.code = WARNING_TEMP_HIGH;
          }
        }
      }
      if (is_err) {
        msg_create_generic(&msg, sizeof(msg_error), MSG_ERROR, &err);
        osMessageQueuePut(CAN_QHandle, &msg, NULL, 0);
      } else
      if (is_warn) {
        msg_create_generic(&msg, sizeof(msg_warning), MSG_WARNING, &warn);
        osMessageQueuePut(CAN_QHandle, &msg, NULL, 0);
      }

    }

    if (last_fuel + frequency[CONTROL_FUEL] < osKernelGetTickCount()) {
      //do the fuel polling, really easy, but also don't know if this will work at the end of the day
    }

    //have to do the same thing here as i have to do in the other task
    
  }
  /* USER CODE END Start_Polling_handler */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

