
#ifndef __CWM_MSG_QUEUE_H
#define __CWM_MSG_QUEUE_H

void CWM_MSG_QUEUE_INIT(void);
int CWM_MSG_QUEUE_REGISTERED(uint32_t evtHandle, void *evtData, CWM_EVT_CALLBACK evtCbfunc);
void CWM_MSG_QUEUE_LISTEN(void);
int CWM_MSG_QUEUE_SEND(pCWM_CMD_t data);
int CWM_MSG_QUEUE_SEND_ISR(pCWM_CMD_t data);

#endif /* __CWM_MSG_QUEUE_H */
