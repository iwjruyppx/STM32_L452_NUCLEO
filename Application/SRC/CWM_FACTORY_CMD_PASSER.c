
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "CWM_FACTORY_CMD_PASSER.h"

static int SelftTestAcc_Cb(struct CWM_FACTORY_INFO_t *info, void *data)
{
    return CWM_NON;
}

static int SelftTestGyro_Cb(struct CWM_FACTORY_INFO_t *info, void *data)
{
    return CWM_NON;
}

static int SelftTestMag_Cb(struct CWM_FACTORY_INFO_t *info, void *data)
{
    return CWM_NON;
}

const CWM_FACTORY_t FACTORY_TABLE[] =
{
    SIGN_FUN_INTO_FACTCMD_T("$selftest Acc: on", FACTORY_TYPE_FACTORY_MODE, FACTORY_CMD_SELFTEST, FACTORY_OPC_ACCELEROMETER, SelftTestAcc_Cb)
    SIGN_FUN_INTO_FACTCMD_T("$selftest Gyro: on", FACTORY_TYPE_FACTORY_MODE, FACTORY_CMD_SELFTEST, FACTORY_OPC_GYROSCOPE, SelftTestGyro_Cb)
    SIGN_FUN_INTO_FACTCMD_T("$selftest Mag: on", FACTORY_TYPE_FACTORY_MODE, FACTORY_CMD_SELFTEST, FACTORY_OPC_MAGNETOMETER, SelftTestMag_Cb)
};

#define SIZE_OF_FACTORY_TABLE \
    (sizeof(FACTORY_TABLE) / sizeof(CWM_FACTORY_t))

int CWM_FACTORY_CMD_SEARCH(uint8_t * data)
{
    int i, index;

    for(i=0;i<SIZE_OF_FACTORY_TABLE;i++)
    {
        index = CWM_STRING_COMPARE((char*)data, (char*)FACTORY_TABLE[i].stringCmd);
        
        if(index >= 0)
        {
            return FACTORY_TABLE[i].cmdCallBack((struct CWM_FACTORY_INFO_t *)&FACTORY_TABLE[i], (void *)data);
        }
    }
    return CWM_ERROR_CMD_NOT_FOUND;
}

int CWM_FACTORY_CMD_PASSER_INIT(void)
{
    return CWM_NON;
}

