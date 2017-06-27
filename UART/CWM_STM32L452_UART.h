
#ifndef __CWM_STM32L452_UART_H
#define __CWM_STM32L452_UART_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Cube Hal include file*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

#ifdef USE_UART4_PA0_PA1
/* User can use this section to tailor CWM_UART4/UARTx instance used and associated 
   resources */
/* Definition for CWM_UART4 clock resources */
#define CWM_UART4                           UART4
#define CWM_UART4_CLK_ENABLE()              __HAL_RCC_UART4_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA2_CLK_ENABLE()
#define CWM_UART4_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define CWM_UART4_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define CWM_UART4_FORCE_RESET()             __HAL_RCC_UART4_FORCE_RESET()
#define CWM_UART4_RELEASE_RESET()           __HAL_RCC_UART4_RELEASE_RESET()

/* Definition for CWM_UART4 Pins */
#define CWM_UART4_TX_PIN                    GPIO_PIN_0
#define CWM_UART4_TX_GPIO_PORT              GPIOA
#define CWM_UART4_TX_AF                     GPIO_AF8_UART4
#define CWM_UART4_RX_PIN                    GPIO_PIN_1
#define CWM_UART4_RX_GPIO_PORT              GPIOA
#define CWM_UART4_RX_AF                     GPIO_AF8_UART4

/* Definition for CWM_UART4's DMA */
#define CWM_UART4_TX_DMA_CHANNEL             DMA2_Channel3
#define CWM_UART4_RX_DMA_CHANNEL             DMA2_Channel5

/* Definition for CWM_UART4's DMA Request */
#define CWM_UART4_TX_DMA_REQUEST             DMA_REQUEST_2
#define CWM_UART4_RX_DMA_REQUEST             DMA_REQUEST_2

/* Definition for CWM_UART4's NVIC */
#define CWM_UART4_DMA_TX_IRQn                DMA2_Channel3_IRQn
#define CWM_UART4_DMA_RX_IRQn                DMA2_Channel5_IRQn
#define CWM_UART4_DMA_TX_IRQHandler          DMA2_Channel3_IRQHandler
#define CWM_UART4_DMA_RX_IRQHandler          DMA2_Channel5_IRQHandler

/* Definition for CWM_UART4's NVIC */
#define CWM_UART4_IRQn                      UART4_IRQn
#define CWM_UART4_IRQHandler                UART4_IRQHandler

#endif

/* Size of Reception buffer */
#define RXBUFFERSIZE                      128
  
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */


extern UART_HandleTypeDef CWM_UART4_Handle;

void CWM_UART_INIT(void);
int CWM_UADR_READ(uint8_t * RxBuffer, int RxBufferSize, CWM_UART_CALLBACK pRxCallBack);
int CWM_UADR_WRITE(uint8_t * TxBuffer, int TxBufferSize, CWM_UART_CALLBACK pTxCallBack );
int CWM_UART_LISTEN(int RxBufferSize, CWM_UART_CALLBACK pTxCallBack);

#endif /* __CWM_STM32L452_I2C_H */
