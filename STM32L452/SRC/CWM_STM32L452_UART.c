
#include "CWM_STM32L452_UART.h"
#include "CWM_STM32L452_UART4.h"
#include "CWM_STM32L452_USART3_PB4_PB5.h"
#include "CWM_UART_QUEUE.h"
#include "CWM_MSG_QUEUE.h"

#ifdef USE_UART
/* Size of Reception buffer */
#define MAX_RXBUFFERSIZE                      128

typedef struct {
    int status;
    int size;
    int errCode;
    uint8_t RxBuffer[MAX_RXBUFFERSIZE];
    CWM_STRING_CALLBACK callBack;
}CWM_UART_LISTEN_t, *pCWM_UART_LISTEN_t;

CWM_STRING_CALLBACK UartTxCallBack = NULL;

#ifdef USE_UART4_PA0_PA1
CWM_UART_LISTEN_t listen_uart4;

int CWM_UART4_LISTEN(int RxBufferSize, CWM_STRING_CALLBACK pTxCallBack)
{
    if(listen_uart4.status == 0)
    {
        listen_uart4.status = 1;
        listen_uart4.size = RxBufferSize;
        listen_uart4.callBack = pTxCallBack;
        return CWM_UART4_READ(listen_uart4.RxBuffer, listen_uart4.size);
    }
    return CWM_ERROR_ALREADY_INITIAL;
}
#endif

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of DMA Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
uint8_t text1RxBuffer[255] ;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#ifdef USE_UART4_PA0_PA1
    if(huart->Instance == UART4)
    {
        /* Set transmission flag: trasfer complete*/
        if(listen_uart4.status)
        {
            listen_uart4.errCode = 0;
            if(listen_uart4.callBack != NULL)
                listen_uart4.callBack(listen_uart4.RxBuffer, listen_uart4.size);
            
            CWM_UART4_READ(listen_uart4.RxBuffer, listen_uart4.size);
            CWM_UART_QUEUE_SET(listen_uart4.RxBuffer, listen_uart4.size);
            
            CWM_CMD_t data;
            data.cmd = CWM_CMD_UART_LISTEN_RX_UPDATE;
            CWM_MSG_QUEUE_SEND(&data);
        }
    }
#endif
    if(huart->Instance == USART3)
    {
        CWM_USART3_READ(text1RxBuffer, 32);
    }
}

void HAL_UART_RxCpltEntryCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART3)
    {
    }
}

/**
  * @brief  UART error callbacks
  * @param  huart: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
#ifdef USE_UART4_PA0_PA1
    if(huart->Instance == UART4)
    {
        if(listen_uart4.status)
        {
            listen_uart4.errCode = -1;
            if(listen_uart4.callBack != NULL)
                listen_uart4.callBack(listen_uart4.RxBuffer, -1);

            
            CWM_UART4_READ(listen_uart4.RxBuffer, listen_uart4.size);
        }
    }
#endif
    if(huart->Instance == USART3)
    {
        CWM_USART3_READ(text1RxBuffer, 32);
    }
}

/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
#ifdef USE_UART4_PA0_PA1
        HAL_UART_MspInit_UART4_PA0_PA1(huart);
#endif
#ifdef USE_USART3_PB4_PB5
    HAL_UART_MspInit_USART3_PB4_PB5(huart);
#endif

}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */


void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
#ifdef USE_UART4_PA0_PA1
    HAL_UART_MspDeInit_UART4_PA0_PA1(huart);
#endif

#ifdef USE_USART3_PB4_PB5
    HAL_UART_MspDeInit_USART3_PB4_PB5(huart);
#endif
}

static void evtcb_CWM_CMD_UART_LISTEN(void *handle, void *evtData)
{
#ifdef USE_UART4_PA0_PA1
    CWM_UART4_LISTEN(64, NULL);
#endif
}

static void CWM_UART_EVENT_REGISTER(void)
{
    /*Uart cmd: listen register*/
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_UART_LISTEN, NULL, evtcb_CWM_CMD_UART_LISTEN);
}

void CWM_UART_INIT(void)
{
    /*Uart info queue*/
    CWM_UART_QUEUE_INIT();

    /*Event listen register*/
    CWM_UART_EVENT_REGISTER();

#ifdef USE_UART4_PA0_PA1
    CWM_UART_INIT_UART4_PA0_PA1();
#endif

#ifdef USE_USART3_PB4_PB5
    CWM_UART_INIT_USART3_PB4_PB5();

#endif


}
#else
void CWM_UART_INIT(void)
{
}
#endif

