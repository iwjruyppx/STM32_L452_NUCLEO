
#ifndef __CWM_STM32L452_USART_H
#define __CWM_STM32L452_USART_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Cube Hal include file*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo.h"

/* User can use this section to tailor USARTx/UARTx instance used and associated 
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART2
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA2_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_2
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_3
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART2

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_CHANNEL             DMA2_Channel3
#define USARTx_RX_DMA_CHANNEL             DMA2_Channel5

/* Definition for USARTx's DMA Request */
#define USARTx_TX_DMA_REQUEST             DMA_REQUEST_2
#define USARTx_RX_DMA_REQUEST             DMA_REQUEST_2

/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn                DMA2_Channel3_IRQn
#define USARTx_DMA_RX_IRQn                DMA2_Channel5_IRQn
#define USARTx_DMA_TX_IRQHandler          DMA2_Channel3_IRQHandler
#define USARTx_DMA_RX_IRQHandler          DMA2_Channel5_IRQHandler

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler

/* Size of Reception buffer */
#define RXBUFFERSIZE                      128
  
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */


extern UART_HandleTypeDef UartHandle;

void CWM_UART_INIT(void);
int CWM_UADR_READ(uint8_t * RxBuffer, int RxBufferSize, CWM_UART_CALLBACK pRxCallBack);
int CWM_UADR_WRITE(uint8_t * TxBuffer, int TxBufferSize, CWM_UART_CALLBACK pTxCallBack );
int CWM_UART_LISTEN(int RxBufferSize, CWM_UART_CALLBACK pTxCallBack);

#endif /* __CWM_STM32L452_USART_H */
