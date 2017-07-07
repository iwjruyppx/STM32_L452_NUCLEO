
#include "CWM_TASK1.h"

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_MSG_QUEUE.h"
#include "CWM_FACTORY_CMD_PASSER.h"

#define Task_Name "CWMTask1"

extern int64_t gTimestamp;

#ifdef USE_USART3_PB4_PB5
#include "CWM_STM32L452_USART3_PB4_PB5.h"
ModuleHc06Class_t BLE_HC06S;
#define BT_SEND(fmt, args...) \
    if(NULL != BLE_HC06S.sendData)\
        BLE_HC06S.sendData(&BLE_HC06S, fmt, ##args)

static void evtcb_CWM_CMD_UART_LISTEN_RX_UPDATE(void *handle, void *evtData)
{
    pCWM_CMD_t data = (pCWM_CMD_t)evtData;
    int rtv ;

    
    rtv = CWM_FACTORY_CMD_SEARCH((uint8_t *)data->string);
    if( rtv<0)
    { 
        BT_SEND("%s\r\n", data->string);
    }
}

static void BT_INIT(void)
{
    pUsartClass_t ptr = CWM_GET_USART_HANDLE(CWM_USART3);
    if(ptr != NULL)
        BLE_HC06S.stringWriteCallBack = ptr->write;
    
    MODULE_HC_06_INIT(&BLE_HC06S);

    /*Register CWM_CMD_USART3_RX_UPDATE event callback*/
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_USART3_RX_UPDATE, NULL, evtcb_CWM_CMD_UART_LISTEN_RX_UPDATE);

}
#else
__weak static void BT_INIT(void)
{
}
#endif/*USE_USART3_PB4_PB5*/

static void Task1(const void *argument)
{  
    
    for (;;)
    {
        CWM_MSG_QUEUE_LISTEN();
    }
}

void CWM_TASK1_INIT(void)
{

    osThreadDef(Task_Name, Task1, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(Task_Name), NULL);

    BT_INIT();
}
