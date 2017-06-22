
#include "CWM_MSG_QUEUE.h"
#include "CWM_CMD_PASSER.h"

#define CMD_QUEUE_SIZE 16
static osMessageQId CWM_MSG_QUEUE;

void CWM_MSG_QUEUE_INIT(void)
{
    /* Create the queue used by the two threads */
    osMessageQDef(CMD_QUEUE, CMD_QUEUE_SIZE, CWM_CMD_t);
    CWM_MSG_QUEUE = osMessageCreate(osMessageQ(CMD_QUEUE), NULL);

}

void CWM_MSG_QUEUE_LISTEN(void)
{
    osStatus status;
    CWM_CMD_t data;
    
    status =  osMessageGetMem (CWM_MSG_QUEUE, &data, osWaitForever);
    if (status == osEventMessage)
        MsgPasser(&data);
    
}

int CWM_MSG_QUEUE_SEND(pCWM_CMD_t data)
{
    if (osMessagePutMem(CWM_MSG_QUEUE, data, 100) != osOK)
    {
        return CWM_CMD_FAIL_SEND_TO_QUEUE;
    }
    return 0;
}