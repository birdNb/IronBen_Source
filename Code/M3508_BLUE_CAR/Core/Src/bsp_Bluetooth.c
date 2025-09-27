/*
 * 本文件的功能是实现蓝牙的一些功能，包括通信，以及发送的指令解析
 *  使用此文件请配置好蓝牙对应的串口，否则可能会失效
 *  在使用本文件时，请调用Bluetooth_uart_init()，在main函数里
 *  更改日期：2023-09-15
 */

#include "bsp_Bluetooth.h"
#include "string.h"
#include "stdio.h"



Blue_DataTypedef blueData;

/*******************************************************************************************
  * @Func		areArraysEqual(const uint8_t *arr1, const uint8_t *arr2,uint16_t size)
  * @Brief    主要比较两个长度相等数组是否相等
  * @Param		const uint8_t *arr1, const uint8_t *arr2,uint16_t size
  * @Retval		相等true          不相等false
  * @Date     2023-09-15
 *******************************************************************************************/
static bool areArraysEqual(const uint8_t *arr1, const uint8_t *arr2,uint16_t size){
    for(int i = 0;i < size;i++){
        if(arr1[i] != arr2[i]){
            return false;
        }
    }
    return true;
}

static void toLowerCase(const uint8_t *original, uint8_t *result) {
  int i = 0;
  for (i = 0; original[i] != '\0'; i++) {
    if (original[i] >= 'A' && original[i] <= 'Z') {
      result[i] = original[i] | 32;
    } else {
      result[i] = original[i];
    }
  }
  result[i] = '\0'; // 添加字符串结束符
}


/*******************************************************************************************
  * @Func		HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t size);
  * @Brief    串口接收中断回调函数，不用声明，产生串口接收中断会调用
  * @Param		UART_HandleTypeDef *huart(串口句柄)     uint16_t size(接收数据的大小)
  * @Retval		无
  * @Date     2023-09-15
 *******************************************************************************************/

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t size){
//    if(huart == &BLUETOOTH_UART){
//        blueData.Rx_len = size;
////        Bluetooth_process(size);
//			BlueTooth_YanGan(size);
//        memset(blueData.Data,0,BUFFER_SIZE);
//        blueData.Rx_len = 0;
//        HAL_UARTEx_ReceiveToIdle_IT(&BLUETOOTH_UART,blueData.Data,BUFFER_SIZE);
//    }
//		if(huart == &huart8){
//			blueData.Rx_len = size;
//			ESP32_DATA(size);
//			memset(blueData.Data,0,BUFFER_SIZE);
//      blueData.Rx_len = 0;
//      HAL_UARTEx_ReceiveToIdle_IT(&huart8,blueData.Data,BUFFER_SIZE);
//		}
		if(huart == &huart6){
			if(blueData.mode == 1){
				blueData.Rx_len = size;
				ESP32_DATA(size);
				memset(blueData.Data,0,BUFFER_SIZE);
				blueData.Rx_len = 0;
			}		
      HAL_UARTEx_ReceiveToIdle_IT(&huart6,blueData.Data,BUFFER_SIZE);
		}
		

}

/*******************************************************************************************
  * @Func		Bluetooth_uart_init()
  * @Brief    为蓝牙串口做初始化，使用此文件时，必须在main函数中调用
  * @Param		无
  * @Retval		无
  * @Date     2023-09-15
 *******************************************************************************************/
void Bluetooth_uart_init(){
    HAL_UARTEx_ReceiveToIdle_IT(&BLUETOOTH_UART,blueData.Data,BUFFER_SIZE);
		HAL_UARTEx_ReceiveToIdle_IT(&huart8,blueData.Data,BUFFER_SIZE);
		HAL_UARTEx_ReceiveToIdle_IT(&huart6,blueData.Data,BUFFER_SIZE);
}

/*******************************************************************************************
  * @Func		Bluetooth_process()
  * @Brief    处理蓝牙接收到的数据
  * @Param		无
  * @Retval		无
  * @Date     2023-09-15
 *******************************************************************************************/
void Bluetooth_process(uint16_t len){
   uint8_t rowData[len];
   for(int i = 0;i<len;i++){
       rowData[i] = blueData.Data[i];
   }
    if(areArraysEqual(rowData,(uint8_t *)"forward",len))
		{
			set_speed0 = 2000;//目标速度
			set_speed1 = -2000;//目标速度
			set_speed2 = -2000;//目标速度
			set_speed3 = 2000;//目标速度
//			set_speed(0,3000,0);
        printf("\n前进");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"back",len))
		{
			set_speed0 = -2000;//目标速度
			set_speed1 = 2000;//目标速度
			set_speed2 = 2000;//目标速度
			set_speed3 = -2000;//目标速度
//				set_speed(0,-3000,0);
      printf("\n后退");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"left",len))
		{
//			set_speed0 = -1500;//目标速度
//			set_speed1 = -1500;//目标速度
//			set_speed2 = 1500;//目标速度
//			set_speed3 = 1500;//目标速度
			set_speed(-3000,0,0);
      printf("\n向左");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"right",len))
		{
//			set_speed0 = 1500;//目标速度
//			set_speed1 = 1500;//目标速度
//			set_speed2 = -1500;//目标速度
//			set_speed3 = -1500;//目标速度
			set_speed(3000,0,0);
      printf("\n向右");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"forward,left",len))
		{
//			set_speed0 = -3000;//目标速度
//			set_speed1 = -3000;//目标速度
//			set_speed2 = -3000;//目标速度
//			set_speed3 = -3000;//目标速度
				set_speed(-2000,2000,0);

        printf("\n前左");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"forward,right",len))
		{
//			set_speed0 = 3000;//目标速度
//			set_speed1 = 3000;//目标速度
//			set_speed2 = 3000;//目标速度
//			set_speed3 = 3000;//目标速度
				set_speed(2000,2000,0);
        printf("\n前右");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"back,right",len))
		{
					
			set_speed(2000,-2000,0);
			printf("\n后右");
			
    }
    else if(areArraysEqual(rowData,(uint8_t *)"back,left",len))
		{
			set_speed(-2000,-2000,0);
      printf("\n后左");
    }
		 else if(areArraysEqual(rowData,(uint8_t *)"Protate",len))
		{
//			set_speed0 = 0;//目标速度
//			set_speed1 = 0;//目标速度
//			set_speed2 = 0;//目标速度
//			set_speed3 = 0;//目标速度
				set_speed(0,0,3000);
        printf("\n正旋转");
    }
		else if(areArraysEqual(rowData,(uint8_t *)"Crotate",len))
		{
//			set_speed0 = 0;//目标速度
//			set_speed1 = 0;//目标速度
//			set_speed2 = 0;//目标速度
//			set_speed3 = 0;//目标速度
				set_speed(0,0,-3000);
        printf("\n逆旋转");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"kuangbao",len))
		{
//			set_speed0 = 0;//目标速度
//			set_speed1 = 0;//目标速度
//			set_speed2 = 0;//目标速度
//			set_speed3 = 0;//目标速度
				set_speed(0,2500,2500);
        printf("\n都给我死");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"stop",len))
		{
//			set_speed0 = 0;//目标速度
//			set_speed1 = 0;//目标速度
//			set_speed2 = 0;//目标速度
//			set_speed3 = 0;//目标速度
				set_speed(0,0,0);
        printf("\n暂停");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"off",len))
		{
        printf("\n关闭电机电源");
    }

}

void BlueTooth_YanGan(uint16_t len)
	{
		uint8_t rowData[len];
		int BX = 0,BY = 0,BW = 0;
		printf("%s\r\n",blueData.Data);
		if(sscanf(blueData.Data,"X%dY%d",&BX,&BY)==2)
		{
			printf("X: %d\r\n",BX);
			printf("Y: %d\r\n",BY);

		}
		else
		{
			for(int i = 0;i<len;i++)
			{
				rowData[i] = blueData.Data[i];
				if(areArraysEqual(rowData,(uint8_t *)"Protate",len))
				{
					BW=3000;
				}
				else if(areArraysEqual(rowData,(uint8_t *)"Crotate",len))
				{
					BW=-3000;
				}
				else if(areArraysEqual(rowData,(uint8_t *)"stop",len))
				{
					BW=0;
				}
		}
	}
	if(blueData.Data==0) {BX=0;BY=0;BW=0;}
			set_speed(BX*15,-BY*15,BW);
}
void ESP32_DATA(uint16_t len) {
  int BX = 0, BY = 0, BW = 0;
  uint8_t rowData[len];
  if (sscanf(blueData.Data, "X%dY%dW%d", &BX, &BY, &BW) == 3) {
//    HAL_UART_Transmit(&huart7, (uint8_t *)"OK", sizeof("OK"), HAL_MAX_DELAY);
		printf("%s",blueData.Data);
		set_speed(BX*15,BY*15,-BW*15);
  }
	
    /*切换模式*/
  else {
    toLowerCase(blueData.Data, rowData);
    /*自动模式*/
    if (areArraysEqual(rowData, (uint8_t *)"auto", len)) {
      HAL_UART_Transmit(&huart7, (uint8_t *)"Sure? (Y/N)", sizeof("Sure?(Y/N)"), HAL_MAX_DELAY);
      blueData.flag = 1;
    }
      /*手动模式*/
    else if (areArraysEqual(rowData, (uint8_t *)"manual", len)) {
      HAL_UART_Transmit(&huart7, (uint8_t *)"Sure? (Y/N)", sizeof("Sure?(Y/N)"), HAL_MAX_DELAY);
      blueData.flag = 2;
    } else if (areArraysEqual(rowData, (uint8_t *)"mode?", len)) {
      if (blueData.mode == 1) {
       HAL_UART_Transmit(&huart7, (uint8_t *)"MODE: auto", sizeof("MODE: auto"), HAL_MAX_DELAY);
      } else {
       HAL_UART_Transmit(&huart7, (uint8_t *)"MODE: manual", sizeof("MODE: manual"), HAL_MAX_DELAY);
      }

    }
    if (blueData.flag == 1) {
      if (areArraysEqual(rowData, (uint8_t *)"yes", len)) {
       blueData.mode = 1;
       HAL_UART_Transmit(&huart7, (uint8_t *)"OK", sizeof("OK"), HAL_MAX_DELAY);
       HAL_UART_Transmit(&huart7, (uint8_t *)"MODE: auto", sizeof("MODE: auto"), HAL_MAX_DELAY);
      } else if (areArraysEqual(rowData, (uint8_t *)"no", len)) {
       blueData.mode = 0;
       HAL_UART_Transmit(&huart7, (uint8_t *)"MODE: manual", sizeof("MODE: manual"), HAL_MAX_DELAY);
      }
    }
    if (blueData.flag == 2) {
      if (areArraysEqual(rowData, (uint8_t *)"yes", len)) {
       blueData.mode = 0;
       HAL_UART_Transmit(&huart7, (uint8_t *)"OK", sizeof("OK"), HAL_MAX_DELAY);
      }
    }
		

  }

}






