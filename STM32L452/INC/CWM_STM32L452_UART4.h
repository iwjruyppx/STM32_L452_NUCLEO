
#ifndef __CWM_STM32L452_UART4_H
#define __CWM_STM32L452_UART4_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Cube Hal include file*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

#ifdef USE_UART4_PA0_PA1
pUsartClass_t CWM_GET_UART4_ENTRY(void);
#endif

#endif /* __CWM_STM32L452_UART4_H */
