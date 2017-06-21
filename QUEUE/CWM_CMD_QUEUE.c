
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "cmsis_os.h"
#include "CWM_CMD_QUEUE.h"
#include "CWM_CMD_PASSER.h"

#define CMD_QUEUE_SIZE 16
static osMessageQId CWM_CMD_QUEUE;

void CWM_CMD_QUEUE_INIT(void)
{
    /* Create the queue used by the two threads */
    osMessageQDef(CMD_QUEUE, CMD_QUEUE_SIZE, CWM_CMD_t);
    CWM_CMD_QUEUE = osMessageCreate(osMessageQ(CMD_QUEUE), NULL);

}

void CWM_CMD_QUEUE_LISTEN(void)
{
    osStatus status;
    CWM_CMD_t data;
    
    status =  osMessageGetMem (CWM_CMD_QUEUE, &data, osWaitForever);
    if (status == osEventMessage)
        CmdPasser(&data);
    
}


osStatus osMessagePutMem(osMessageQId queue_id, void *info, uint32_t millisec);
osStatus osMessageGetMem (osMessageQId queue_id, void *info, uint32_t millisec);

int CWM_CMD_QUEUE_SEND(pCWM_CMD_t data)
{
    if (osMessagePutMem(CWM_CMD_QUEUE, data, 100) != osOK)
    {
        return CWM_CMD_FAIL_SEND_TO_QUEUE;
    }
    return 0;
}

int CWM_CMD_QUEUE_SEND_ISR(pCWM_CMD_t data)
{
    if (osMessagePutMem(CWM_CMD_QUEUE, data, 100) != osOK)
    {
        return CWM_CMD_FAIL_SEND_TO_QUEUE;
    }
    return 0;
}