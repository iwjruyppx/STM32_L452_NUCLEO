
#include "CWM_TASK1.h"

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "CWM_CMD_QUEUE.h"
#include "CWM_MSG_QUEUE.h"

#define Task_Name "CWMTask1"

int task1_state = 0;
extern int64_t gTimestamp;
int time = 0;

int sizesss = 0;

#if 0
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
#endif

static void statemachine(void)
{
    CWM_CMD_t data;
 
    switch(task1_state)
    {
#if 0
        case 0 :
            data.cmd = CWM_CMD_SCREEN_INIT;
            CWM_CMD_QUEUE_SEND(&data);
            break;
        case 1 :
            data.cmd = CWM_CMD_SCREEN_ON;
            CWM_CMD_QUEUE_SEND(&data);
            
            data.cmd = CWM_CMD_UART_LISTEN;
            CWM_CMD_QUEUE_SEND(&data);
            break;
#else
        case 0 :
            data.cmd = CWM_CMD_UART_LISTEN;
            CWM_CMD_QUEUE_SEND(&data);
            break;
            
#endif
        default:
            time = gTimestamp/1000;
        //    LOGE("CurrentT:%d\n",time);
            CWM_MSG_QUEUE_LISTEN();
            
            break;
    }
    task1_state++;
}

static void Task1(const void *argument)
{  
    for (;;)
    {
        statemachine();
    }
}

void CWM_TASK1_INIT(void)
{
    osThreadDef(Task_Name, Task1, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(Task_Name), NULL);
}
