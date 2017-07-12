
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_STM32L452_I2C.h"
#include "CWM_STM32L452_I2C1.h"

typedef struct {
    pI2cClass_t i2c;
}CWM_I2C_CONTROL_t;

static CWM_I2C_CONTROL_t CWMI2C1;

#if 0
void HAL_I2C_EV_IRQHandler(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ER_IRQHandler(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c);
#else
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(NULL != CWMI2C1.i2c)
        CWMI2C1.i2c->TxCpltCallback(hi2c);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if(NULL != CWMI2C1.i2c)
        CWMI2C1.i2c->ErrorCallback(hi2c);
}

#endif


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


