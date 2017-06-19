
#ifndef __CWM_MSG_QUEUE_H
#define __CWM_MSG_QUEUE_H

#include "CWM_UTILITY.h"

void CWM_MSG_QUEUE_INIT(void);
void CWM_MSG_QUEUE_LISTEN(void);
int CWM_MSG_QUEUE_SEND(pCWM_CMD_t data);
int CWM_MSG_QUEUE_SEND_ISR(pCWM_CMD_t data);

#endif /* __CWM_MSG_QUEUE_H */
