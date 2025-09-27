/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "head.h"
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

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId LcdShowHandle;
osThreadId Send_Rec_msgHandle;
osSemaphoreId myBinarySem01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void LcdShow_02(void const * argument);
void Send_Rec_msg_03(void const * argument);

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
  /* definition and creation of myBinarySem01 */
  osSemaphoreDef(myBinarySem01);
  myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LcdShow */
  osThreadDef(LcdShow, LcdShow_02, osPriorityIdle, 0, 128);
  LcdShowHandle = osThreadCreate(osThread(LcdShow), NULL);

  /* definition and creation of Send_Rec_msg */
  osThreadDef(Send_Rec_msg, Send_Rec_msg_03, osPriorityIdle, 0, 128);
  Send_Rec_msgHandle = osThreadCreate(osThread(Send_Rec_msg), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
        key_scan();
    }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_LcdShow_02 */
/**
 * @brief Function implementing the LcdShow thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_LcdShow_02 */
void LcdShow_02(void const * argument)
{
  /* USER CODE BEGIN LcdShow_02 */
    /* Infinite loop */
    for (;;)
    {
        osDelay(1);
        lcd_disp();
        
    }
  /* USER CODE END LcdShow_02 */
}

/* USER CODE BEGIN Header_Send_Rec_msg_03 */
/**
 * @brief Function implementing the Send_Rec_msg thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Send_Rec_msg_03 */
void Send_Rec_msg_03(void const * argument)
{
  /* USER CODE BEGIN Send_Rec_msg_03 */

    /* Infinite loop */
    for (;;)
    {
      imu_prog();
      send_order();
      osDelay(35); // 延时10ms
    }
  /* USER CODE END Send_Rec_msg_03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

