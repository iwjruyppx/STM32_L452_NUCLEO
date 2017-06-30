
#ifndef __MODULE_BT_HC_06_SLAVE_H
#define __MODULE_BT_HC_06_SLAVE_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#ifdef USE_MODULE_BT_HC_06_SLAVE

typedef enum {
    CMD_HC06_TEST = 0,
    CMD_HC06_GET_VERSION,
    CMD_HC06_SET_NAME,
    CMD_HC06_SET_PIN,
    CMD_HC06_SET_BAUD_1200,
    CMD_HC06_SET_BAUD_2400,
    CMD_HC06_SET_BAUD_4800,
    CMD_HC06_SET_BAUD_9600,
    CMD_HC06_SET_BAUD_19200,
    CMD_HC06_SET_BAUD_38400,
    CMD_HC06_SET_BAUD_57600,
    CMD_HC06_SET_BAUD_115200,
    CMD_HC06_SET_BAUD_230400,
    CMD_HC06_SET_BAUD_460800,
    CMD_HC06_SET_BAUD_921600,
    CMD_HC06_SET_BAUD_1382400,
    CMD_HC06_MAX
}HC06_CMD_e;

typedef struct ModuleHc06Class{
    int (*setCmd)(struct ModuleHc06Class *, HC06_CMD_e, uint8_t *);
    
    /*User need to implement this API*/
    int (*stringWriteCallBack)(uint8_t *,int);
    /*User need to implement this API End*/
} ModuleHc06Class_t, *pModuleHc06Class_t;

void MODULE_HC_06_INIT(pModuleHc06Class_t handle);

#endif /*USE_MODULE_BT_HC_06_SLAVE*/

#endif /* __MODULE_BT_HC_06_SLAVE_H */
