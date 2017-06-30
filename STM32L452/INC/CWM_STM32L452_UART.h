
#ifndef __CWM_STM32L452_UART_H
#define __CWM_STM32L452_UART_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Cube Hal include file*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

typedef enum {
    CWM_USART1 = 0,
    CWM_USART2,
    CWM_USART3,
    CWM_UART4,
    CWM_USART_MAX,
}CWM_UART_e;

int GET_GPS_UART_STRING(void);
void CWM_UART_INIT(void);

#endif /* __CWM_STM32L452_I2C_H */
