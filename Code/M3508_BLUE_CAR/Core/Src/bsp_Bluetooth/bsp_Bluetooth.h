#ifndef VET6_UART_BSP_BLUETOOTH_H
#define VET6_UART_BSP_BLUETOOTH_H

#ifdef STM32F4
#include "stm32f4xx_hal.h"
#elif defined STM32F1
#include "stm32f1xx_hal.h"
#endif
#include "usart.h"
#include "stdbool.h"
//蓝牙连接的串口号
#define     BLUETOOTH_UART             huart2
//蓝牙串口接收DMA通道，如果要使用，请开启DMA
#define     BLUETOOTH_UART_DMA hdma_usart1_rx
//接收数据的缓冲大小
#define     BUFFER_SIZE                     50

typedef struct {
    uint8_t Data[BUFFER_SIZE];
    uint16_t Rx_len;
}Blue_DataTypedef;

extern Blue_DataTypedef blueData;

void Bluetooth_process(uint16_t len);
void Bluetooth_uart_init();

#endif //VET6_UART_BSP_BLUETOOTH_H
