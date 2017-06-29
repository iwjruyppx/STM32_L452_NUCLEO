
#ifndef __CWM_STM32L452_TIMER_TIM3_H
#define __CWM_STM32L452_TIMER_TIM3_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Cube Hal include file*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

#ifdef USE_TIMER3
pTimerClass_t CWM_GET_TIMER3_ENTRY(void);

#endif  //USE_TIMER3

#endif /* __CWM_STM32L452_TIMER_TIM3_H */
