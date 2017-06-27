
#ifndef __CWM_STM32L452_UART_H
#define __CWM_STM32L452_UART_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Cube Hal include file*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

void CWM_UART_INIT(void);
int CWM_UADR_READ(uint8_t * RxBuffer, int RxBufferSize, CWM_UART_CALLBACK pRxCallBack);
int CWM_UADR_WRITE(uint8_t * TxBuffer, int TxBufferSize, CWM_UART_CALLBACK pTxCallBack );
int CWM_UART_LISTEN(int RxBufferSize, CWM_UART_CALLBACK pTxCallBack);

#endif /* __CWM_STM32L452_I2C_H */
