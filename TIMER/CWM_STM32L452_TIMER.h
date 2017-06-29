
#ifndef __CWM_STM32L452_TIMER_H
#define __CWM_STM32L452_TIMER_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Cube Hal include file*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

#ifdef USE_TIMER_PWM_PULSE
    #define TIMx                             TIM1

    /* Definition for TIMx clock resources */
    #define TIMx_CLK_ENABLE                  __HAL_RCC_TIM1_CLK_ENABLE
    #define DMA_TIMER_CLK_ENABLE                  __HAL_RCC_DMA1_CLK_ENABLE

    /* Definition for TIMx Pins */
    #define TIMx_CHANNEL3_GPIO_CLK_ENABLE    __HAL_RCC_GPIOA_CLK_ENABLE

    #define TIMx_GPIO_CHANNEL3_PORT          GPIOA
    #define GPIO_PIN_CHANNEL3                GPIO_PIN_10


    #define GPIO_AF_TIMx                     GPIO_AF1_TIM1

    /* Definition for TIMx's DMA */
    #define TIMx_CC3_DMA_REQUEST             DMA_REQUEST_7
    #define TIMx_CC3_DMA_INST                DMA1_Channel7

    /* Definition for ADCx's NVIC */
    #define TIMx_DMA_IRQn                    DMA1_Channel7_IRQn
    #define TIMx_DMA_IRQHandler              DMA1_Channel7_IRQHandler

    extern TIM_HandleTypeDef    CWMTimHandle;

#endif  //USE_TIMER_PWM_PULSE

void CWM_TIMER_INIT(void);

#endif /* __CWM_STM32L452_I2C_H */
