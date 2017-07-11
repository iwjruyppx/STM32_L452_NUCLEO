
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_STM32L452_I2C.h"
#include "CWM_STM32L452_I2C1.h"

typedef struct {
    pI2cClass_t i2c;
}CWM_I2C1_CONTROL_t;

static CWM_I2C1_CONTROL_t CWMI2C1;


void CWM_I2C_INIT(void)
{
    CWMI2C1.i2c = CWM_GET_I2C1_ENTRY();
    CWMI2C1.i2c->init();
}

pI2cClass_t CWM_GET_I2C_HANDLE(CWM_I2C_e dev)
{
    switch(dev)
    {
        case CWM_I2C1:
            return CWMI2C1.i2c;
        default:
            break;
    }
    return NULL;
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C1)
    {
        if(NULL != CWMI2C1.i2c)
            CWMI2C1.i2c->mspInit(hi2c);
    }

}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C1)
    {
        if(NULL != CWMI2C1.i2c)
            CWMI2C1.i2c->mspDeInit(hi2c);
    }
}


