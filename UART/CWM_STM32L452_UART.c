
#include "CWM_STM32L452_UART.h"
#include "CWM_UART_QUEUE.h"
#include "CWM_MSG_QUEUE.h"

typedef struct {
    int status;
    int size;
    int errCode;
    uint8_t RxBuffer[RXBUFFERSIZE];
    CWM_UART_CALLBACK callBack;
}CWM_UART_LISTEN_t, *pCWM_UART_LISTEN_t;

UART_HandleTypeDef UartHandle;
__IO ITStatus UartReady = RESET;

uint8_t aTxBuffer[RXBUFFERSIZE] = " 1234567 ";
int TxSize = 0;

CWM_UART_CALLBACK UartTxCallBack = NULL;

CWM_UART_LISTEN_t listen;

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
  CWM_UART_QUEUE_INIT();

}


int CWM_UADR_READ(uint8_t * RxBuffer, int RxBufferSize, CWM_UART_CALLBACK pRxCallBack)
{
    HAL_StatusTypeDef status;
    listen.size = RxBufferSize;
    
    status = HAL_UART_Receive_DMA(&UartHandle, listen.RxBuffer, listen.size);
    return status;
}

int CWM_UADR_READ1(void)
{
    return HAL_UART_Receive_DMA(&UartHandle, listen.RxBuffer, listen.size);
}


int CWM_UADR_WRITE(uint8_t * TxBuffer, int TxBufferSize, CWM_UART_CALLBACK pTxCallBack )
{
    HAL_StatusTypeDef status;
    if(RXBUFFERSIZE <= TxBufferSize)
        return -1;
        
    UartTxCallBack = pTxCallBack;
    TxSize = TxBufferSize;

    memcpy(aTxBuffer, TxBuffer, TxBufferSize);
    
    status = HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)aTxBuffer, TxBufferSize);
    return status;
}

int CWM_UART_LISTEN(int RxBufferSize, CWM_UART_CALLBACK pTxCallBack)
{
    HAL_StatusTypeDef status;

    listen.status = 1;
    listen.size = RxBufferSize;
    listen.callBack = pTxCallBack;
    
    status = HAL_UART_Receive_DMA(&UartHandle, listen.RxBuffer, listen.size);
    
    return status;
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
    if(listen.status)
    {
        listen.errCode = 0;
        if(listen.callBack != NULL)
            listen.callBack(listen.RxBuffer, listen.size);
        
        CWM_UADR_READ1();
        CWM_UART_QUEUE_SET(listen.RxBuffer, listen.size);
        
        CWM_CMD_t data;
        data.cmd = CWM_CMD_UART_LISTEN_RX_UPDATE;
        CWM_MSG_QUEUE_SEND(&data);
    }
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
    //Error_Handler();
    if(listen.status)
    {
        listen.errCode = -1;
        if(listen.callBack != NULL)
            listen.callBack(listen.RxBuffer, -1);
        
        CWM_UADR_READ1();
    }
}

int CWM_UART_QUEUE_SET(uint8_t *data, int size);
int CWM_UART_QUEUE_GET(uint8_t *data, int size);
int CWM_UART_QUEUE_GET_COUNT(void);

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
