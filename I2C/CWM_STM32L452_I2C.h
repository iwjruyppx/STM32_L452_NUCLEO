
#ifndef __CWM_STM32L452_I2C_H
#define __CWM_STM32L452_I2C_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Cube Hal include file*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 80 MHz */
/* This example use TIMING to 0x00D00E28 to reach 1 MHz speed (Rise time = 120ns, Fall time = 25ns) */
//#define I2C_TIMING      0x00D00E28      //1000K
#define I2C_TIMING      0x10D1143A      //400K

#if (defined (USE_STM32L0XX_NUCLEO))
#define NUCLEO_I2C_EXPBD_TIMING_100KHZ       0x10A13E56 /* Analog Filter ON, Rise Time 400ns, Fall Time 100ns */
#define NUCLEO_I2C_EXPBD_TIMING_400KHZ       0x00B1112E /* Analog Filter ON, Rise Time 250ns, Fall Time 100ns */
#endif /* USE_STM32L0XX_NUCLEO */

/* Timing samples for L4 with SYSCLK 80MHz set in SystemClock_Config() */
#if (defined (USE_STM32F7XX_NUCLEO))
#define NUCLEO_I2C_EXPBD_TIMING_400KHZ       0x10D1143A /* Analog Filter ON, Rise time 250ns, Fall Time 100ns */
#define NUCLEO_I2C_EXPBD_TIMING_1000KHZ      0x00D00E28 /* Analog Filter ON, Rise time 120ns, Fall time 25ns */
#endif /* USE_STM32F7XX_NUCLEO */


#define I2C_ADDRESS        0x30F

/* User can use this section to tailor I2Cx/I2Cx instance used and associated
   resources */
/* Definition for I2Cx clock resources */
#define I2Cx                            I2C1
#define RCC_PERIPHCLK_I2Cx              RCC_PERIPHCLK_I2C1
#define RCC_I2CxCLKSOURCE_SYSCLK        RCC_I2C1CLKSOURCE_SYSCLK
#define I2Cx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 
#define I2Cx_DMA_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()

#define I2Cx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_8
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SDA_PIN                    GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SCL_SDA_AF                 GPIO_AF4_I2C1

/* Definition for I2Cx's NVIC */
#define I2Cx_EV_IRQn                    I2C1_EV_IRQn
#define I2Cx_ER_IRQn                    I2C1_ER_IRQn
#define I2Cx_EV_IRQHandler              I2C1_EV_IRQHandler
#define I2Cx_ER_IRQHandler              I2C1_ER_IRQHandler

/* Definition for I2Cx's DMA */
#define I2Cx_DMA                        DMA1   
#define I2Cx_DMA_INSTANCE_TX            DMA1_Channel2
#define I2Cx_DMA_INSTANCE_RX            DMA1_Channel3
#define I2Cx_DMA_REQUEST_TX             DMA_REQUEST_3
#define I2Cx_DMA_REQUEST_RX             DMA_REQUEST_3

/* Definition for I2Cx's DMA NVIC */
#define I2Cx_DMA_TX_IRQn                DMA1_Channel2_IRQn
#define I2Cx_DMA_RX_IRQn                DMA1_Channel3_IRQn
#define I2Cx_DMA_TX_IRQHandler          DMA1_Channel2_IRQHandler
#define I2Cx_DMA_RX_IRQHandler          DMA1_Channel3_IRQHandler

extern I2C_HandleTypeDef I2cHandle;

void CWM_I2C_INIT(void);
void CWM_I2C_MASTER_DMA_INIT(void);
void CWM_I2C_MASTER_DMA_DeINIT(void);

void CWM_I2CMASTER_DMA_WRITE_REG_SINGLE(uint16_t DevAddress, uint8_t Reg, uint8_t sData);
void CWM_I2CMASTER_DMA_WRITE_REG(uint16_t DevAddress, uint8_t Reg, uint8_t *pData, uint16_t Size);
void CWM_I2CMASTER_DMA_WRITE(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
void CWM_I2CMASTER_DMA_READ(uint16_t DevAddress, uint8_t *pData, uint16_t Size);

#endif /* __CWM_STM32L452_I2C_H */
