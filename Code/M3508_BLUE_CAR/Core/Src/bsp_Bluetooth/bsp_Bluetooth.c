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

/*******************************************************************************************
  * @Func		HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t size);
  * @Brief    串口接收中断回调函数，不用声明，产生串口接收中断会调用
  * @Param		UART_HandleTypeDef *huart(串口句柄)     uint16_t size(接收数据的大小)
  * @Retval		无
  * @Date     2023-09-15
 *******************************************************************************************/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t size){
    if(huart == &BLUETOOTH_UART){
        blueData.Rx_len = size;
        Bluetooth_process(size);
        memset(blueData.Data,0,BUFFER_SIZE);
        blueData.Rx_len = 0;
        HAL_UARTEx_ReceiveToIdle_IT(&BLUETOOTH_UART,blueData.Data,BUFFER_SIZE);
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
    if(areArraysEqual(rowData,(uint8_t *)"forward",len)){
        printf("\n前进4！");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"back",len)){
        printf("\n后退！后退");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"forward,left",len)){
        printf("\n前左");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"forward,right",len)){
        printf("\n前右");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"back,right",len)){
        printf("\n后右");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"back,left",len)){
        printf("\n后左");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"left",len)){
        printf("\n向左");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"right",len)){
        printf("\n向右");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"stop",len)){
        printf("\n暂停");
    }
    else if(areArraysEqual(rowData,(uint8_t *)"off",len)){
        printf("\n关闭电机电源");
    }

}






