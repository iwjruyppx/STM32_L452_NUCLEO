
#include "CWM_TASK1.h"

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_CMD_QUEUE.h"
#include "CWM_MSG_QUEUE.h"

#define Task_Name "CWMTask1"

extern int64_t gTimestamp;

static void evtcb_CWM_CMD_UART_LISTEN_RX_UPDATE(void *handle, void *evtData)
{
    pCWM_CMD_t data = (pCWM_CMD_t)evtData;
    printf("%s\n", data->string);
}

static void Task1(const void *argument)
{  
    #if 0
    CWM_CMD_t data;
    data.cmd = CWM_CMD_USART_LISTEN;
    data.device = CWM_UART4;
    CWM_MSG_QUEUE_SEND(&data);
    #endif
    CWM_CMD_t data;
    
    data.cmd = CWM_CMD_USART_LISTEN;
    data.device = CWM_USART3;
    CWM_MSG_QUEUE_SEND(&data);
    
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
}
