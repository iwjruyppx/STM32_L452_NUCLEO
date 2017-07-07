
#ifndef __CWM_STM32L452_UART4_H
#define __CWM_STM32L452_UART4_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Cube Hal include file*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

#ifdef USE_UART4_PA0_PA1
void CWM_UART4_INIT(pUsartClass_t init);
#endif

#endif /* __CWM_STM32L452_UART4_H */
