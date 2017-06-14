
#ifndef __CWM_CMD_QUEUE_H
#define __CWM_CMD_QUEUE_H

#include "CWM_UTILITY.h"

void CWM_CMD_QUEUE_INIT(void);
void CWM_CMD_QUEUE_LISTEN(void);
int CWM_CMD_QUEUE_SEND(pCWM_CMD_t data);

#endif /* __CWM_CMD_QUEUE_H */
