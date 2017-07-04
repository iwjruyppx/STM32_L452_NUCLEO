
#include "CWM_TASK1.h"

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_STM32L452_USART3_PB4_PB5.h"

#include "CWM_MSG_QUEUE.h"

#define Task_Name "CWMTask1"

ModuleHc06Class_t BLE_HC06S;
extern int64_t gTimestamp;

static void evtcb_CWM_CMD_UART_LISTEN_RX_UPDATE(void *handle, void *evtData)
{
    pCWM_CMD_t data = (pCWM_CMD_t)evtData;
    int rtv ;
    
    rtv = CWM_STRING_COMPARE(data->string, "$selftest Gyro: on");
    if( rtv>=0)
    { 
        BLE_HC06S.sendData(&BLE_HC06S,"%s\r\n", data->string);
    }
}

static void Task1(const void *argument)
{  

    /*Enable Usart3 RX listen*/
    CWM_INTERNAL_CMD_SET_CT(CWM_CMD_USART_LISTEN, CWM_USART3);

    /*Register CWM_CMD_USART3_RX_UPDATE event callback*/
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_USART3_RX_UPDATE, NULL, evtcb_CWM_CMD_UART_LISTEN_RX_UPDATE);
    
    for (;;)
    {
        CWM_MSG_QUEUE_LISTEN();
    }
}

void CWM_TASK1_INIT(void)
{

    osThreadDef(Task_Name, Task1, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(Task_Name), NULL);
    
    MODULE_HC_06_INIT(&BLE_HC06S);
    BLE_HC06S.stringWriteCallBack = CWM_USART3_WRITE;
}
