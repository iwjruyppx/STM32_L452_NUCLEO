
#include "CWM_STM32L452_UART.h"
#include "CWM_STM32L452_UART4.h"
#include "CWM_STM32L452_USART3_PB4_PB5.h"
#include "CWM_STM32L452_TIMER.h"

#include "CWM_UART_QUEUE.h"
#include "CWM_MSG_QUEUE.h"

#ifdef USE_UART
/* Size of Reception buffer */
#define MAX_RXBUFFERSIZE                      128

#ifdef USE_USART3_PB4_PB5
typedef struct {
    int status;
    int time_en;
    pTimerClass_t tim;
    CwmQueue_t queue;
    uint8_t RxBuffer[MAX_CWM_CMD_DATA_SIZE];
}CWM_USART3_CONTROL_t;

static CWM_USART3_CONTROL_t CWM_U3;
#endif /*USE_USART3_PB4_PB5*/

#ifdef USE_UART4_PA0_PA1
typedef struct {
    int status;
    int size;
    int errCode;
    uint8_t RxBuffer[MAX_RXBUFFERSIZE];
    CWM_STRING_CALLBACK callBack;
    CwmQueue_t queue;
}CWM_UART_LISTEN_t, *pCWM_UART_LISTEN_t;

CWM_UART_LISTEN_t CWM_U4;

#endif /*USE_UART4_PA0_PA1*/

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
  
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    CWM_CMD_t data;
    
#ifdef USE_UART4_PA0_PA1
    if(huart->Instance == UART4)
    {
        /* Set transmission flag: trasfer complete*/
        if(CWM_U4.status)
        {
            CWM_U4.errCode = 0;
            if(CWM_U4.callBack != NULL)
                CWM_U4.callBack(CWM_U4.RxBuffer, CWM_U4.size);
            
            CWM_UART_QUEUE_SET(&CWM_U4.queue, CWM_U4.RxBuffer, CWM_U4.size);
            CWM_UART4_READ(CWM_U4.RxBuffer, CWM_U4.size);
            
            data.cmd = CWM_CMD_UART4_RX_UPDATE;
            CWM_MSG_QUEUE_SEND(&data);
        }
    }
#endif /*USE_UART4_PA0_PA1*/

#ifdef USE_USART3_PB4_PB5
    if(huart->Instance == USART3)
    {
        if(CWM_U3.time_en)
        {
            CWM_U3.tim->stop();
            CWM_U3.tim->resetCount();
            CWM_U3.time_en = 0;
        }

        if(CWM_U3.status)
        {
            /*Send data to host*/
            data.cmd = CWM_CMD_USART3_RX_UPDATE;
            memcpy(data.string, CWM_U3.RxBuffer, MAX_CWM_CMD_DATA_SIZE-3);
            data.string[MAX_CWM_CMD_DATA_SIZE -3] = 0x0D; /*0x0D = "\r" */
            data.string[MAX_CWM_CMD_DATA_SIZE -2] = 0x0A; /*0x0A = "\n" */
            data.string[MAX_CWM_CMD_DATA_SIZE -1] = 0x00; /*0x00 = "\0" */
            CWM_MSG_QUEUE_SEND(&data);
        
            CWM_USART3_READ(CWM_U3.RxBuffer, MAX_CWM_CMD_DATA_SIZE-3);
        }
    }
#endif /*USE_USART3_PB4_PB5*/
}

void HAL_UART_RxCpltEntryCallback(UART_HandleTypeDef *huart)
{
#ifdef USE_USART3_PB4_PB5
    if(huart->Instance == USART3)
    {
        if(!CWM_U3.time_en)
        {
            CWM_U3.time_en = 1;
            CWM_U3.tim->start();
        }
        else
            CWM_U3.tim->resetCount();
    }
#endif /*USE_USART3_PB4_PB5*/
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
        if(CWM_U4.status)
        {
            CWM_U4.errCode = -1;
            if(CWM_U4.callBack != NULL)
                CWM_U4.callBack(CWM_U4.RxBuffer, -1);
            
            CWM_UART4_READ(CWM_U4.RxBuffer, CWM_U4.size);
        }
    }
#endif /*USE_UART4_PA0_PA1*/
#ifdef USE_USART3_PB4_PB5
    if(huart->Instance == USART3)
    {
    
        if(CWM_U3.time_en)
        {
            CWM_U3.tim->stop();
            CWM_U3.tim->resetCount();
            CWM_U3.time_en = 0;
        }
        
        HAL_UART_AbortReceive_IT(huart);
        CWM_UART_DEINIT_USART3_PB4_PB5();
        CWM_UART_INIT_USART3_PB4_PB5();
        
        if(CWM_U3.status)
        {
            CWM_USART3_READ(CWM_U3.RxBuffer, MAX_CWM_CMD_DATA_SIZE-3);
        }
    }
#endif /*USE_USART3_PB4_PB5*/
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
#endif /*USE_UART4_PA0_PA1*/
#ifdef USE_USART3_PB4_PB5
    HAL_UART_MspInit_USART3_PB4_PB5(huart);
#endif /*USE_USART3_PB4_PB5*/

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
#endif /*USE_UART4_PA0_PA1*/

#ifdef USE_USART3_PB4_PB5
    HAL_UART_MspDeInit_USART3_PB4_PB5(huart);
#endif /*USE_USART3_PB4_PB5*/
}

static void evtcb_CWM_CMD_USART_LISTEN(void *handle, void *evtData)
{
    pCWM_CMD_t cmd = (pCWM_CMD_t)evtData;
#ifdef USE_UART4_PA0_PA1
    if(CWM_UART4 == cmd->device)
    {
        if(CWM_U4.status == 0)
        {
            CWM_U4.status = 1;
            CWM_U4.size = 64;
            CWM_UART4_READ(CWM_U4.RxBuffer, CWM_U4.size);
        }
    }
#endif /*USE_UART4_PA0_PA1*/

#ifdef USE_USART3_PB4_PB5
    if(CWM_USART3 == cmd->device)
    {
        if(CWM_U3.status == 0)
        {
            CWM_U3.status = 1;
            CWM_USART3_READ(CWM_U3.RxBuffer, MAX_CWM_CMD_DATA_SIZE-3);
        }
    }
#endif /*USE_USART3_PB4_PB5*/
}

static void CWM_UART_EVENT_REGISTER(void)
{
    /*Uart cmd: listen register*/
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_USART_LISTEN, NULL, evtcb_CWM_CMD_USART_LISTEN);
}

int timestamp = 0;

static void CWM_TIME3_IRQ_CALLBACK(void *info)
{
    CWM_CMD_t data;
    int i = 0;
    int j = 0;

    if(CWM_U3.time_en)
    {
        CWM_U3.tim->stop();
        CWM_U3.tim->resetCount();
        CWM_U3.time_en = 0;
    }
    
    if(CWM_U3.status)
    {
        UART_HandleTypeDef *pHandle = CWM_USART3_GET_HANDLE();
        
        /*Send data to host*/
        data.cmd = CWM_CMD_USART3_RX_UPDATE;
        for(i=0; i<pHandle->RxXferSize -pHandle->RxXferCount; i++)
        {
            data.string[j++] =  CWM_U3.RxBuffer[i];
        }
        data.string[j++] = 0x0D; /*0x0D = "\r" */
        data.string[j++] = 0x0A; /*0x0A = "\n" */
        data.string[j++] = 0x00; /*0x00 = "\0" */
            
        
        HAL_UART_AbortReceive_IT(pHandle);
        memset(CWM_U3.RxBuffer, 0x00, MAX_CWM_CMD_DATA_SIZE);
        
        CWM_MSG_QUEUE_SEND(&data);
        CWM_USART3_READ(CWM_U3.RxBuffer, MAX_CWM_CMD_DATA_SIZE-3);
    }

    timestamp++;
}

int GET_GPS_UART_STRING(void)
{
    uint8_t data;
    if(CWM_UART_QUEUE_GET(&CWM_U4.queue, &data, 1))
        return -1;
    return data;
}

void CWM_UART_INIT(void)
{
#ifdef USE_UART4_PA0_PA1
    /*Uart info queue*/
    CWM_UART_QUEUE_INIT(&CWM_U4.queue, 1024);

    /*Event listen register*/
    CWM_UART_EVENT_REGISTER();

    CWM_UART_INIT_UART4_PA0_PA1();
#endif /*USE_UART4_PA0_PA1*/

#ifdef USE_USART3_PB4_PB5
    UART_HandleTypeDef *pHandle;
    float dt = 0.0f;

    /*Uart info queue*/
    CWM_UART_QUEUE_INIT(&CWM_U3.queue, 256);

    CWM_UART_DEINIT_USART3_PB4_PB5();
    CWM_UART_INIT_USART3_PB4_PB5();

    /*Timer3 get entry and register callback*/
    CWM_U3.tim = CWM_GET_TIMER_ENTRY(CWM_TIMER3);
    CWM_TIMER_REG_CALL_BACK(CWM_TIMER3, CWM_TIME3_IRQ_CALLBACK);
    CWM_U3.tim->deInit();
    CWM_U3.tim->init();
    
    pHandle = CWM_USART3_GET_HANDLE();

    /*
        9600 1byte about 1ms
        buff time reserve  3time, about 3ms.
    */
    if(pHandle->Init.BaudRate !=0)
    {
        dt = (9600.0f /(float)pHandle->Init.BaudRate) *3.0f;
        CWM_U3.tim->setPeriod((dt < 1.0f) ? 1:(uint32_t)dt);
    }
    CWM_U3.tim->stop();
    CWM_U3.tim->resetCount();
    
#endif /*USE_USART3_PB4_PB5*/

}
#else
void CWM_UART_INIT(void)
{
}
#endif /*USE_UART*/

