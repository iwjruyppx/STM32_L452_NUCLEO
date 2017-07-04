
#include "CWM_INTERNAL_CMD.h"
#include "CWM_MSG_QUEUE.h"

int CWM_INTERNAL_CMD(void)
{
    return CWM_NON;
}

int CWM_INTERNAL_CMD_SET_T(pCWM_CMD_t pdata)
{
    return CWM_MSG_QUEUE_SEND(pdata);
}

int CWM_INTERNAL_CMD_SET(uCWM_CMD cmd)
{
    CWM_CMD_t data;
    data.cmd = cmd;
    return CWM_INTERNAL_CMD_SET_T(&data);
}
int CWM_INTERNAL_CMD_SET_CT(uCWM_CMD cmd, int type)
{
    CWM_CMD_t data;
    data.cmd = cmd;
    data.type = type;
    return CWM_INTERNAL_CMD_SET_T(&data);
}

int CWM_INTERNAL_CMD_SET_V(uCWM_CMD cmd, void *info)
{
    CWM_CMD_t data;
    
    switch (cmd)
    {
        case CWM_CMD_USART_LISTEN:
            break;
        default:
            return CWM_ERROR_CMD_NOT_FOUND;
    }
    data.cmd = cmd;
    CWM_MSG_QUEUE_SEND(&data);
    
    return CWM_NON;
}

    