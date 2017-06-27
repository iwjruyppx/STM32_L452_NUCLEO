
#ifndef __CWM_STM32L452_UART4_H
#define __CWM_STM32L452_UART4_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Cube Hal include file*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

#ifdef USE_UART4_PA0_PA1
void CWM_UART_INIT_UART4_PA0_PA1(void);
void HAL_UART_MspInit_UART4_PA0_PA1(UART_HandleTypeDef *huart);
void HAL_UART_MspDeInit_UART4_PA0_PA1(UART_HandleTypeDef *huart);

int CWM_UART4_WRITE(uint8_t * TxBuffer, int TxBufferSize);
int CWM_UART4_READ(uint8_t * RxBuffer, int RxBufferSize);
#endif

#endif /* __CWM_STM32L452_UART4_H */
