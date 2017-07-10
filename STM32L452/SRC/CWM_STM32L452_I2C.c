
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_STM32L452_I2C.h"

/* I2C handler declaration */
I2C_HandleTypeDef I2cHandle;

extern int64_t gTimestamp;

void CWM_I2C_INIT(void)
{
    CWM_I2C_MASTER_DMA_INIT();
}

/******************************************************************************/
/*                 STM32L4xx Peripherals Interrupt Handlers                  */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l4xx.s).                                               */
/******************************************************************************/

void I2Cx_EV_IRQHandler(void);
void I2Cx_ER_IRQHandler(void);
void I2Cx_DMA_RX_IRQHandler(void);
void I2Cx_DMA_TX_IRQHandler(void);
/**
  * @brief  This function handles I2C event interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to I2C data transmission
  */
void I2Cx_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&I2cHandle);
}

/**
  * @brief  This function handles I2C error interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to I2C error
  */
void I2Cx_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&I2cHandle);
}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA Channel 
  *         used for I2C data transmission     
  */
void I2Cx_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(I2cHandle.hdmarx);
}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA Channel 
  *         used for I2C data reception    
  */
void I2Cx_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(I2cHandle.hdmatx);
}

void CWM_I2C_MASTER_DMA_INIT(void)
{
  
  /*##-1- Configure the I2C peripheral ######################################*/
  I2cHandle.Instance              = I2Cx;
  I2cHandle.Init.Timing           = I2C_TIMING;
  I2cHandle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;  
  I2cHandle.Init.OwnAddress1      = I2C_ADDRESS;
  I2cHandle.Init.OwnAddress2      = 0xFF;
  
  if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Enable the Analog I2C Filter */
  HAL_I2CEx_ConfigAnalogFilter(&I2cHandle,I2C_ANALOGFILTER_ENABLE);
}

void CWM_I2C_MASTER_DMA_DeINIT(void)
{
  if(HAL_I2C_DeInit(&I2cHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

void CWM_I2CMASTER_DMA_WRITE_REG_SINGLE(uint16_t DevAddress, uint8_t Reg, uint8_t sData){
  uint8_t data[2];
  data[0] = Reg;
  data[1] = sData;
  CWM_I2CMASTER_DMA_WRITE(DevAddress, data, 2);  
}


void CWM_I2CMASTER_DMA_WRITE_REG(uint16_t DevAddress, uint8_t Reg, uint8_t *pData, uint16_t Size){
  uint8_t data[255];
  data[0] = Reg;
  memcpy(&data[1], pData, Size);
  CWM_I2CMASTER_DMA_WRITE(DevAddress, data, Size + 1);  
}

void CWM_I2CMASTER_DMA_WRITE(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    uint64_t inTime;
    inTime = gTimestamp+10;
  
  /*##-2- Start the transmission process #####################################*/  
  /* While the I2C in reception process, user can transmit data through 
     "aTxBuffer" buffer */
  do
  {
    if(HAL_I2C_Master_Transmit(&I2cHandle, DevAddress, pData, Size, 10)!= HAL_OK)
    {
      /* Error_Handler() function is called when error occurs. */
//      Error_Handler();
    }

    /*##-3- Wait for the end of the transfer #################################*/  
    /*  Before starting a new communication transfer, you need to check the current   
        state of the peripheral; if it’s busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the 
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */  
    while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
    {
        if(gTimestamp >= inTime)
        {
            CWM_I2C_MASTER_DMA_DeINIT();
            CWM_I2C_MASTER_DMA_INIT();
            break;
        }
    } 

    /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
       Master restarts communication */
  }
  while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);
  
  
}
  
void CWM_I2CMASTER_DMA_READ(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    uint64_t inTime;
    inTime = gTimestamp+10;
  do
  {
    if(HAL_I2C_Master_Receive(&I2cHandle, DevAddress, pData, Size, 100) != HAL_OK)
    {
      /* Error_Handler() function is called when error occurs. */
      Error_Handler();
    }

    /*##-5- Wait for the end of the transfer #################################*/  
    /*  Before starting a new communication transfer, you need to check the current   
        state of the peripheral; if it’s busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the 
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */  
    while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
    {
        if(gTimestamp >= inTime)
        {
            CWM_I2C_MASTER_DMA_DeINIT();
            CWM_I2C_MASTER_DMA_INIT();
            break;
        }
    } 

    /* When Acknowledge failure occurs (Slave don't acknowledge it's address)
       Master restarts communication */
  }
  while(HAL_I2C_GetError(&I2cHandle) == HAL_I2C_ERROR_AF);

}