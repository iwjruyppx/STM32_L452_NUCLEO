
#ifndef __CWM_FACTORY_CMD_PASSER_H
#define __CWM_FACTORY_CMD_PASSER_H

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

typedef enum {    
    FACTORY_TYPE_RESERVED = 0,
    FACTORY_TYPE_FACTORY_MODE,
    FACTORY_TYPE_DEBUG_MODE,
}uCWM_FACTORY_TYPE;

typedef enum {    
    FACTORY_CMD_RESERVED = 0,
    FACTORY_CMD_SELFTEST,
    FACTORY_CMD_CALIB,
    FACTORY_CMD_LINEAR_CALIB,
}uCWM_FACTORY_CMD;

typedef enum {    
    FACTORY_OPC_RESERVED = 0,
    FACTORY_OPC_ACCELEROMETER,
    FACTORY_OPC_GYROSCOPE,
    FACTORY_OPC_MAGNETOMETER,
    FACTORY_OPC_BAROMETER,
    FACTORY_OPC_AMBIENT_LIGHT,
    FACTORY_OPC_PROXIMITY,
    FACTORY_OPC_GESTURE,
    FACTORY_OPC_TEMPERATURE,
    FACTORY_OPC_UVLIGHT,
    FACTORY_OPC_HRM,
    FACTORY_OPC_LEDs,
    FACTORY_OPC_BUTTONS,
    FACTORY_OPC_GPIOS,
    FACTORY_OPC_FLASH,
    FACTORY_OPC_EEPROM,
    FACTORY_OPC_TOUCH,
    FACTORY_OPC_OLED,
}uCWM_FACTORY_OPCODE;

typedef struct CWM_FACTORY_INFO_t{
    uint8_t type;
    uint8_t cmdId;
    uint8_t opCode;
    uint8_t *stringCmd;
    int (*cmdCallBack)(struct CWM_FACTORY_INFO_t *, void *);
}CWM_FACTORY_t, pCWM_FACTORY_t;

#define SIGN_FUN_INTO_FACTCMD_T(sc, t, ci, oc, cb)  \
{                                                   \
        .stringCmd = (uint8_t *)sc,                   \
        .type = (uint8_t)t,                   \
        .cmdId = (uint8_t)ci,                   \
        .opCode = (uint8_t)oc,                   \
        .cmdCallBack = cb,                   \
},


int CWM_FACTORY_CMD_SEARCH(uint8_t * data);

int CWM_FACTORY_CMD_PASSER_INIT(void);

#endif /* __CWM_FACTORY_CMD_PASSER_H */
