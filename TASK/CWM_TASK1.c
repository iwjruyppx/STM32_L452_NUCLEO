
#include "CWM_TASK1.h"

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "CWM_CMD_QUEUE.h"
#include "CWM_MSG_QUEUE.h"

#define Task_Name "CWMTask1"

extern int64_t gTimestamp;

static void Task1(const void *argument)
{  
    CWM_CMD_t data;
    data.cmd = CWM_CMD_UART_LISTEN;
    CWM_MSG_QUEUE_SEND(&data);
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
