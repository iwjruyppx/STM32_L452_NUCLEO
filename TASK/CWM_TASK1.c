
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>

#include "cmsis_os.h"
#include "CWM_TASK1.h"
#include "CWM_CMD_QUEUE.h"

#define Task_Name "CWMTask1"

int task1_state = 0;
extern int64_t gTimestamp;

int sizesss = 0;

int LOGE(const char * format,...){
    va_list argList;
    CWM_CMD_t data;

    va_start(argList,format);
    vsnprintf(data.string,sizeof(data.string),format,argList);
    va_end(argList);
    
    sizesss = sizeof(data.string);
    data.cmd = CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE;
    CWM_CMD_QUEUE_SEND(&data);
    
    data.cmd = CWM_CMD_SCREEN_UPDATE;
    CWM_CMD_QUEUE_SEND(&data);
    return 0;
}

void statemachine(void)
{
    CWM_CMD_t data;
    char string0[] =  "0bcdefghijk";
    
    switch(task1_state)
    {
        case 0 :
            data.cmd = CWM_CMD_SCREEN_INIT;
            CWM_CMD_QUEUE_SEND(&data);
            break;
        case 1 :
            data.cmd = CWM_CMD_SCREEN_ON;
            CWM_CMD_QUEUE_SEND(&data);
            break;
        case 2 :
            memcpy(data.string, string0,sizeof(string0));
            data.cmd = CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE;
            CWM_CMD_QUEUE_SEND(&data);
            data.cmd = CWM_CMD_SCREEN_UPDATE;
            CWM_CMD_QUEUE_SEND(&data);
            break;
        default:
            LOGE("T:%d%d \n",
            (int32_t)(gTimestamp / 1000000000),
            (int32_t)(gTimestamp % 1000000000)
    );
            break;
    }
    task1_state++;
}

static void Task1(const void *argument)
{  
    for (;;)
    {
        osDelay(1000);
        statemachine();
    }
}

void CWM_TASK1_INIT()
{
    osThreadDef(Task_Name, Task1, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(Task_Name), NULL);
}
