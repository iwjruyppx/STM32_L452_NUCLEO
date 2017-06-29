
#include "CWM_MSG_QUEUE.h"

#define CMD_QUEUE_SIZE 16
static osMessageQId CWM_MSG_QUEUE;

LinkListHandle_t MsgHandle;

int CWM_MSG_QUEUE_REGISTERED(uint32_t evtHandle, void *evtData, CWM_EVT_CALLBACK evtCbfunc)
{
    LinkListData_t registed;
    registed.evtHandle = evtHandle;
    registed.evtData = evtData;
    registed.evtCbfunc = evtCbfunc;
    
    if(NULL == MsgHandle.AddToTail)
        return CWM_ERROR_NULL_POINT;
    return MsgHandle.AddToTail(&MsgHandle, &registed);
}

void CWM_MSG_QUEUE_LISTEN(void)
{
    osStatus status;
    CWM_CMD_t data;
    
    status =  osMessageGetMem (CWM_MSG_QUEUE, &data, osWaitForever);
    if (status == osEventMessage)
        MsgHandle.FindAndCallBack(&MsgHandle, data.cmd, &data);
    
}

int CWM_MSG_QUEUE_SEND(pCWM_CMD_t data)
{
    if (osMessagePutMem(CWM_MSG_QUEUE, data, 100) != osOK)
    {
        return CWM_CMD_FAIL_SEND_TO_QUEUE;
    }
    return 0;
}

void CWM_MSG_QUEUE_INIT(void)
{
    /* Create the queue used by the two threads */
    osMessageQDef(CMD_QUEUE, CMD_QUEUE_SIZE, CWM_CMD_t);
    CWM_MSG_QUEUE = osMessageCreate(osMessageQ(CMD_QUEUE), NULL);

    /*Msg listen initial*/
    MsgHandle.malloc = pvPortMalloc;
    MsgHandle.free= vPortFree;
    CWM_LINK_LIST_INIT(&MsgHandle);

}