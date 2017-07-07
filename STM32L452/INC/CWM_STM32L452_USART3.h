
#ifndef __CWM_STM32L452_USART3_H
#define __CWM_STM32L452_USART3_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Cube Hal include file*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

#ifdef USE_USART3_PB4_PB5

void CWM_USART3_INIT(pUsartClass_t init);

#endif

#endif /* __CWM_STM32L452_USART3_H */
