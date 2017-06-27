
#include "CWM_TASK2.h"

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#define Task_Name "CWMTask2"
#include "CWM_CMD_QUEUE.h"

static void Task2(const void *argument)
{  

    for (;;)
    {
    
    }
}

void CWM_TASK2_INIT(void)
{
    osThreadDef(Task_Name, Task2, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(Task_Name), NULL);
}