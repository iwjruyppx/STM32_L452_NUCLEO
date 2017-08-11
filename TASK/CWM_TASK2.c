
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_TASK2.h"

#define Task_Name "CWMTask2"

#include "CWM_GPS.h"
#include "CWM_STEPPER_MOTOR_PROC.h"

extern int64_t gTimestamp;

static void evtcb_BT_CMD(void *handle, void *evtData)
{
    pCWM_CMD_t data = (pCWM_CMD_t)evtData;
    int degree = 0;

    if(CWM_STRING_COMPARE(data->string, "SM_RotateRight:") >= 0)
    {
        sscanf(data->string, "SM_RotateRight:%d",&degree);
        SetDegrees(degree);
    }
}


static void Task2Start(void)
{
    /*Enable Usart3 RX listen*/
    CWM_INTERNAL_CMD_SET_CT(CWM_CMD_USART_LISTEN, CWM_USART3);
    /*Enable Uart4 RX listen*/
    CWM_INTERNAL_CMD_SET_CT(CWM_CMD_USART_LISTEN, CWM_UART4);
    
    /*OLED Driver Init*/
    CWM_INTERNAL_CMD_SET(CWM_CMD_SCREEN_INIT);
    CWM_INTERNAL_CMD_SET(CWM_CMD_SCREEN_ON);

    /*Register CWM_CMD_USART3_RX_UPDATE event callback*/
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_USART3_RX_UPDATE, NULL, evtcb_BT_CMD);
}

static void Task2(const void *argument)
{ 
    int64_t tempTime;
    Task2Start();
    osDelay(1000);
    for (;;)
    {
        STEPPER_MOTOR_PROC();
        if(tempTime < gTimestamp)
        {
            tempTime = gTimestamp+100;
//            ShowCurrneDegrees();
        }
    }
}

void CWM_TASK2_INIT(void)
{
    osThreadDef(Task_Name, Task2, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(Task_Name), NULL);
}

