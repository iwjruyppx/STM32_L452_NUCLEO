
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "cmsis_os.h"
#include "CWM_TASK2.h"

#define Task_Name "CWMTask2"
#include "CWM_CMD_QUEUE.h"


static void Task2(const void *argument)
{  
    for (;;)
    {
        CWM_CMD_QUEUE_LISTEN();
    }
}

void CWM_TASK2_INIT(void)
{
    osThreadDef(Task_Name, Task2, osPriorityRealtime, 0, configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(Task_Name), NULL);
}