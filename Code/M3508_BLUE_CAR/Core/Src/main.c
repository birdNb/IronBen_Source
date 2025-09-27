/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_can.h"
#include "CAN_receive.h"
#include "bsp_Bluetooth.h"

#include "pid.h"
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

/* USER CODE BEGIN PV */
pid_type_def motor0_pid,motor1_pid,motor2_pid,motor3_pid;	//声明PID数据结构体
const motor_measure_t *motor0_data;//声明电机结构体指针
const motor_measure_t *motor1_data;//声明电机结构体指针
const motor_measure_t *motor2_data;//声明电机结构体指针
const motor_measure_t *motor3_data;//声明电机结构体指针

int set_speed0 = 0;//目标速度
int set_speed1 = 0;//目标速度
int set_speed2 = 0;//目标速度
int set_speed3 = 0;//目标速度
const fp32 PID[3]={13,0.01,6};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void set_speed(int Vx,int Vy,int w)
{
	set_speed1= -(-Vx+Vy-w);//前右目标速度
	set_speed0= Vx+Vy+w;//前左目标速度
	set_speed2= -(Vx+Vy-w);//后右目标速度
	set_speed3= -Vx+Vy+w;//后左目标速度
	
	if(set_speed0>9000){set_speed0=9000;}else if(set_speed0==0){set_speed0=0;}else if(set_speed0<-9000){set_speed0=-9000;}
	if(set_speed1>9000){set_speed1=9000;}else if(set_speed1==0){set_speed1=0;}else if(set_speed1<-9000){set_speed1=-9000;}
	if(set_speed2>9000){set_speed2=9000;}else if(set_speed2==0){set_speed2=0;}else if(set_speed2<-9000){set_speed2=-9000;}
	if(set_speed3>9000){set_speed3=9000;}else if(set_speed3==0){set_speed3=0;}else if(set_speed3<-9000){set_speed3=-9000;}
	
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	blueData.mode = 1;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_UART7_Init();
  MX_USART2_UART_Init();
	MX_UART8_Init();
	MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
	can_filter_init();
	
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);

	PID_init(&motor0_pid,PID_POSITION,PID,16000,2000);//结构体，计算模式，参数，最大输出，最大积分输出
	PID_init(&motor1_pid,PID_POSITION,PID,16000,2000);//结构体，计算模式，参数，最大输出，最大积分输出
	PID_init(&motor2_pid,PID_POSITION,PID,16000,2000);//结构体，计算模式，参数，最大输出，最大积分输出
	PID_init(&motor3_pid,PID_POSITION,PID,16000,2000);//结构体，计算模式，参数，最大输出，最大积分输出
	
	motor0_data = get_chassis_motor_measure_point(0); //获取电机数据指针编号范围（0~3）	
	motor1_data = get_chassis_motor_measure_point(1); //获取电机数据指针编号范围（0~3）
	motor2_data = get_chassis_motor_measure_point(2); //获取电机数据指针编号范围（0~3）
	motor3_data = get_chassis_motor_measure_point(3); //获取电机数据指针编号范围（0~3）
//	printf("Init ok");
	
	Bluetooth_uart_init();
	HAL_Delay(10);
//	CAN_cmd_chassis_reset_ID();
//	HAL_Delay(5000);
//	CAN_cmd_chassis_reset_ID();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		CAN_cmd_chassis(1000, 1000, 1000, 1000);
		PID_calc(&motor0_pid,motor0_data->speed_rpm,set_speed0);//结构体，实际速度，设定速度
		PID_calc(&motor1_pid,motor1_data->speed_rpm,set_speed1);//结构体，实际速度，设定速度
		PID_calc(&motor2_pid,motor2_data->speed_rpm,set_speed2);//结构体，实际速度，设定速度
		PID_calc(&motor3_pid,motor3_data->speed_rpm,set_speed3);//结构体，实际速度，设定速度
		CAN_cmd_chassis(5000,5000,5000,5000);


//printf("%d,%d,%d,%d,%d,%d\r\n",set_speed0,set_speed1,motor0_data->speed_rpm,motor1_data->speed_rpm,motor2_data->speed_rpm,motor3_data->speed_rpm);
		
		HAL_Delay(2);
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
