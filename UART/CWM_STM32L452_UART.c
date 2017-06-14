
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "CWM_STM32L452_UART.h"

UART_HandleTypeDef UartHandle;
__IO ITStatus UartReady = RESET;

uint8_t aTxBuffer[RXBUFFERSIZE] = " ****UART_TwoBoards communication based on DMA****  ****UART_TwoBoards communication based on DMA****  ****UART_TwoBoards communication based on DMA**** ";
uint8_t aRxBuffer[RXBUFFERSIZE];
int TxSize = 0;
int RxSize = 0;

CWM_UART_CALLBACK UartRxCallBack = NULL;
CWM_UART_CALLBACK UartTxCallBack = NULL;

static void Error_Handler(void);


void CWM_UART_INIT(void)
{

  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }

}


int CWM_UADR_READ_WRITE(uint8_t * RxBuffer, CWM_UART_CALLBACK pRxCallBack, 
                                                         uint8_t * TxBuffer, int TxBufferSize, CWM_UART_CALLBACK pTxCallBack )
{
    if(RXBUFFERSIZE <= TxBufferSize)
        return -1;
        
    UartRxCallBack = pRxCallBack;
    UartTxCallBack = pTxCallBack;
    TxSize = TxBufferSize;
    RxSize = TxBufferSize;

    memcpy(aTxBuffer, TxBuffer, TxBufferSize);
    memcpy(aRxBuffer, RxBuffer, TxBufferSize);
    
    if(HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)aRxBuffer, TxBufferSize) != HAL_OK)
    {
        Error_Handler();
    }
    
    if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)aTxBuffer, TxBufferSize)!= HAL_OK)
    {
        Error_Handler();
    }

    return 0;
}



/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of DMA Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: trasfer complete*/
    
    if(UartTxCallBack != NULL)
        UartTxCallBack(aTxBuffer, TxSize);
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: trasfer complete*/
    if(UartRxCallBack != NULL)
        UartRxCallBack(aRxBuffer, RxSize);

}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    Error_Handler();

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* LED2 is slowly blinking (1 sec. period) */
  while(1)
  {    
    BSP_LED_Toggle(LED2); 
    HAL_Delay(1000);
  } 
}
