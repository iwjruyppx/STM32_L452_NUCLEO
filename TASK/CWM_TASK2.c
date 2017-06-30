
#include "CWM_TASK2.h"

/*Standard Utility include file*/
#include "CWM_UTILITY.h"
#include "CWM_CMD_QUEUE.h"
#include "CWM_STM32L452_USART3_PB4_PB5.h"

#define Task_Name "CWMTask2"

ModuleHc06Class_t BLE_HC06S;

static void Task2(const void *argument)
{  

    MODULE_HC_06_INIT(&BLE_HC06S);
    BLE_HC06S.stringWriteCallBack = CWM_USART3_WRITE;
    
    for (;;)
    {
        BLE_HC06S.setCmd(&BLE_HC06S, CMD_HC06_GET_VERSION, NULL);
        osDelay(1000);
    }
}

void CWM_TASK2_INIT(void)
{
    osThreadDef(Task_Name, Task2, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(Task_Name), NULL);
}
