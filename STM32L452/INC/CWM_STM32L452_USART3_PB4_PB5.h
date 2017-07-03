
#ifndef __CWM_STM32L452_USART3_PB4_PB5_H
#define __CWM_STM32L452_USART3_PB4_PB5_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Cube Hal include file*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

#ifdef USE_USART3_PB4_PB5

UART_HandleTypeDef *CWM_USART3_GET_HANDLE(void);
void CWM_UART_INIT_USART3_PB4_PB5(void);
void CWM_UART_DEINIT_USART3_PB4_PB5(void);
void HAL_UART_MspInit_USART3_PB4_PB5(UART_HandleTypeDef *huart);
void HAL_UART_MspDeInit_USART3_PB4_PB5(UART_HandleTypeDef *huart);

int CWM_USART3_WRITE(uint8_t * TxBuffer, int TxBufferSize);
int CWM_USART3_READ(uint8_t * RxBuffer, int RxBufferSize);

#endif

#endif /* __CWM_STM32L452_USART3_PB4_PB5_H */
