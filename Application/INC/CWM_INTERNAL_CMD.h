
#ifndef __CWM_INTERNAL_CMD_H
#define __CWM_INTERNAL_CMD_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

int CWM_INTERNAL_CMD(void);

int CWM_INTERNAL_CMD_SET(uCWM_CMD cmd);
int CWM_INTERNAL_CMD_SET_CT(uCWM_CMD cmd, int type);
int CWM_INTERNAL_CMD_SET_T(pCWM_CMD_t pdata);
int CWM_INTERNAL_CMD_SET_V(uCWM_CMD cmd, void *info);

#endif /* __CWM_INTERNAL_CMD_H */
