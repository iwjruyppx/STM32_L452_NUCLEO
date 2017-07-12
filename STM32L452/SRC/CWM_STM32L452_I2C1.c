
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_STM32L452_I2C1.h"

#define I2C_MAX_QUEUE_BUFF_SIZE 2048
#define MAX_I2C_RW_SIZE 255

typedef struct {
    I2C_HandleTypeDef handle;
    pPackageQueueClass_t pqFunPoint;
    PackageQueue_t qTxMem;
    uint8_t i2cTxBuff[MAX_I2C_RW_SIZE+4];
}CWM_I2C1_CONTROL_t;

/* I2C handler declaration */
static CWM_I2C1_CONTROL_t memI2C1;


/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 80 MHz */
/* This example use TIMING to 0x00D00E28 to reach 1 MHz speed (Rise time = 120ns, Fall time = 25ns) */
//#define I2C_TIMING      0x00D00E28      //1000K
#define I2C_TIMING      0x10D1143A      //400K

#define I2C_ADDRESS        0x30F

/* User can use this section to tailor I2Cx/I2Cx instance used and associated
   resources */
/* Definition for I2Cx clock resources */
#define I2Cx                            I2C1
#define RCC_PERIPHCLK_I2Cx              RCC_PERIPHCLK_I2C1
#define RCC_I2CxCLKSOURCE_SYSCLK        RCC_I2C1CLKSOURCE_SYSCLK
#define I2Cx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 
#define I2Cx_DMA_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()

#define I2Cx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_8
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SDA_PIN                    GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SCL_SDA_AF                 GPIO_AF4_I2C1

/* Definition for I2Cx's NVIC */
#define I2Cx_EV_IRQn                    I2C1_EV_IRQn
#define I2Cx_ER_IRQn                    I2C1_ER_IRQn
#define I2Cx_EV_IRQHandler              I2C1_EV_IRQHandler
#define I2Cx_ER_IRQHandler              I2C1_ER_IRQHandler

/* Definition for I2Cx's DMA */
#define I2Cx_DMA                        DMA1   
#define I2Cx_DMA_INSTANCE_TX            DMA1_Channel6
#define I2Cx_DMA_INSTANCE_RX            DMA1_Channel7
#define I2Cx_DMA_REQUEST_TX             DMA_REQUEST_3
#define I2Cx_DMA_REQUEST_RX             DMA_REQUEST_3

/* Definition for I2Cx's DMA NVIC */
#define I2Cx_DMA_TX_IRQn                DMA1_Channel6_IRQn
#define I2Cx_DMA_RX_IRQn                DMA1_Channel7_IRQn
#define I2Cx_DMA_TX_IRQHandler          DMA1_Channel6_IRQHandler
#define I2Cx_DMA_RX_IRQHandler          DMA1_Channel7_IRQHandler


#define debug_queue 1

#if 1
//#define CWM_I2C_WRITE(d,r,p,s)          HAL_I2C_Master_Transmit(d,r,p,s,100)
#define CWM_I2C_READ(d,r,p,s)          HAL_I2C_Master_Receive(d,r,p,s,100)
#else
#define CWM_I2C_WRITE          HAL_I2C_Master_Transmit_IT
#define CWM_I2C_READ          HAL_I2C_Master_Receive_IT
#endif


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
  HAL_I2C_EV_IRQHandler(&memI2C1.handle);
}

/**
  * @brief  This function handles I2C error interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to I2C error
  */
void I2Cx_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&memI2C1.handle);
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
  HAL_DMA_IRQHandler(memI2C1.handle.hdmarx);
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
  HAL_DMA_IRQHandler(memI2C1.handle.hdmatx);
}

static int CWM_I2C_WRITE(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    uint8_t temp[MAX_I2C_RW_SIZE+4];
    uint16_t qDevAddress;
    uint16_t qSize;
    int ret;
    if(Size > 0)
    {
        if((memI2C1.pqFunPoint->getCount(&memI2C1.qTxMem) ==  0) &&
            (HAL_I2C_GetState(hi2c) == HAL_I2C_STATE_READY))
        {
            memcpy(memI2C1.i2cTxBuff, pData, Size);
            return HAL_I2C_Master_Transmit_IT(hi2c, DevAddress, memI2C1.i2cTxBuff, Size);
        }
        
        /*Add data to queue*/
        memcpy(temp, &DevAddress, sizeof(uint16_t));
        memcpy(&temp[2], &Size, sizeof(uint16_t));
        memcpy(&temp[4], pData, Size);
            
        if(NULL != memI2C1.pqFunPoint)
        {
            ret = memI2C1.pqFunPoint->set(&memI2C1.qTxMem, temp, Size+4);
            if(ret<0)
                return ret;
        }
    }
    
    if(HAL_I2C_GetState(hi2c) == HAL_I2C_STATE_READY)
    {
        ret = memI2C1.pqFunPoint->get(&memI2C1.qTxMem, temp);
        if(ret <= 0)
            return ret;
        memcpy(&qDevAddress, temp, sizeof(uint16_t));
        memcpy(&qSize, &temp[2], sizeof(uint16_t));
        
        memcpy(memI2C1.i2cTxBuff, &temp[4], qSize);
        return HAL_I2C_Master_Transmit_IT(hi2c, qDevAddress, memI2C1.i2cTxBuff, qSize);
    }
    return HAL_OK;
}

static int I2cWrite(uint16_t DevAddress, uint8_t reg, uint8_t *pData, uint16_t Size)
{
    uint8_t data[MAX_I2C_RW_SIZE];
    if((Size+1) >= MAX_I2C_RW_SIZE)
        return HAL_ERROR;
    
    data[0] = reg;
    memcpy(&data[1], pData, Size);
    return CWM_I2C_WRITE(&memI2C1.handle, DevAddress, data, Size+1);
}

static int I2cRead(uint16_t DevAddress, uint8_t reg, uint8_t *pData, uint16_t Size)
{
    return HAL_ERROR;
}

static int I2cBuffWrite(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    return CWM_I2C_WRITE(&memI2C1.handle, DevAddress, pData, Size);
}

static int I2cBuffRead(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    return CWM_I2C_READ(&memI2C1.handle, DevAddress, pData, Size);
}

static int I2cSingleRegWrite(uint16_t DevAddress, uint8_t reg, uint8_t Data)
{
    static uint8_t i2cdData[2];
    i2cdData[0] = reg;
    i2cdData[1] = Data;
    return CWM_I2C_WRITE(&memI2C1.handle, DevAddress, i2cdData, 2);
}

static int I2cSingleRegRead(uint16_t DevAddress, uint8_t reg, uint8_t Data)
{
    return HAL_ERROR;
}

static void HAL_I2C1_MspInit(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    static DMA_HandleTypeDef hdma_tx;
    static DMA_HandleTypeDef hdma_rx;
    RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;

    /*##-1- Configure the I2C clock source. The clock is derived from the SYSCLK #*/
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2Cx;
    RCC_PeriphCLKInitStruct.I2c3ClockSelection = RCC_I2CxCLKSOURCE_SYSCLK;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

    /*##-2- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    I2Cx_SCL_GPIO_CLK_ENABLE();
    I2Cx_SDA_GPIO_CLK_ENABLE();
    /* Enable I2Cx clock */
    I2Cx_CLK_ENABLE();

    /* Enable DMAx clock */
    I2Cx_DMA_CLK_ENABLE();

    /*##-3- Configure peripheral GPIO ##########################################*/  
    /* I2C TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = I2Cx_SCL_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = I2Cx_SCL_SDA_AF;
    HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);

    /* I2C RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = I2Cx_SDA_PIN;
    GPIO_InitStruct.Alternate = I2Cx_SCL_SDA_AF;
    HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);

    /*##-4- Configure the DMA Channels #########################################*/
    /* Configure the DMA handler for Transmission process */
    hdma_tx.Instance                 = I2Cx_DMA_INSTANCE_TX;
    hdma_tx.Init.Request             = I2Cx_DMA_REQUEST_TX;
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_tx.Init.Mode                = DMA_NORMAL;
    hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;

    HAL_DMA_Init(&hdma_tx);   

    /* Associate the initialized DMA handle to the the I2C handle */
    __HAL_LINKDMA(hi2c, hdmatx, hdma_tx);

    /* Configure the DMA handler for Transmission process */
    hdma_rx.Instance                 = I2Cx_DMA_INSTANCE_RX;
    hdma_rx.Init.Request             = I2Cx_DMA_REQUEST_RX;
    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_rx.Init.Mode                = DMA_NORMAL;
    hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;

    HAL_DMA_Init(&hdma_rx);

    /* Associate the initialized DMA handle to the the I2C handle */
    __HAL_LINKDMA(hi2c, hdmarx, hdma_rx);

    /*##-5- Configure the NVIC for DMA #########################################*/
    /* NVIC configuration for DMA transfer complete interrupt (I2Cx_TX) */
    HAL_NVIC_SetPriority(I2Cx_DMA_TX_IRQn, 10, 1);
    HAL_NVIC_EnableIRQ(I2Cx_DMA_TX_IRQn);

    /* NVIC configuration for DMA transfer complete interrupt (I2Cx_RX) */
    HAL_NVIC_SetPriority(I2Cx_DMA_RX_IRQn, 10, 0);   
    HAL_NVIC_EnableIRQ(I2Cx_DMA_RX_IRQn);

    /*##-6- Configure the NVIC for I2C ########################################*/   
    /* NVIC for I2Cx */
    HAL_NVIC_SetPriority(I2Cx_ER_IRQn, 10, 2);
    HAL_NVIC_EnableIRQ(I2Cx_ER_IRQn);
    HAL_NVIC_SetPriority(I2Cx_EV_IRQn, 10, 3);
    HAL_NVIC_EnableIRQ(I2Cx_EV_IRQn);
}

static void HAL_I2C1_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    static DMA_HandleTypeDef hdma_tx;
    static DMA_HandleTypeDef hdma_rx;

    /*##-1- Reset peripherals ##################################################*/
    I2Cx_FORCE_RESET();
    I2Cx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks #################################*/
    /* Configure I2C Tx as alternate function  */
    HAL_GPIO_DeInit(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN);
    /* Configure I2C Rx as alternate function  */
    HAL_GPIO_DeInit(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN);

    /*##-3- Disable the DMA Channels ###########################################*/
    /* De-Initialize the DMA Channel associated to transmission process */
    HAL_DMA_DeInit(&hdma_tx); 
    /* De-Initialize the DMA Channel associated to reception process */
    HAL_DMA_DeInit(&hdma_rx);

    /*##-4- Disable the NVIC for DMA ###########################################*/
    HAL_NVIC_DisableIRQ(I2Cx_DMA_TX_IRQn);
    HAL_NVIC_DisableIRQ(I2Cx_DMA_RX_IRQn);

    /*##-5- Disable the NVIC for I2C ##########################################*/
    HAL_NVIC_DisableIRQ(I2Cx_ER_IRQn);
    HAL_NVIC_DisableIRQ(I2Cx_EV_IRQn);
}

static void evtcb_CWM_CMD_I2C1_LISTEN(void *handle, void *evtData)
{
    pCWM_CMD_t cmd = (pCWM_CMD_t)evtData;
    
    if(cmd->cmd == CWM_CMD_I2C1_TX_CPLT)
    {
        if(NULL != memI2C1.pqFunPoint)
        {
            CWM_I2C_WRITE(&memI2C1.handle, NULL, NULL, 0);
        }
    }
}

static void CWM_I2C1_DEINIT(void)
{
    if(HAL_I2C_DeInit(&memI2C1.handle) != HAL_OK)
    {
        /* Initialization Error */
        Error_Handler();
    }
}

static void CWM_I2C1_INIT(void)
{
    /*Package initial */
    memI2C1.pqFunPoint = CWM_GET_PACKAGE_QUEUE_HANDLE();
    if(NULL != memI2C1.pqFunPoint)
    {
        memI2C1.pqFunPoint->init(&memI2C1.qTxMem, I2C_MAX_QUEUE_BUFF_SIZE);
    }
    /*Event listen register*/
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_I2C1_TX_CPLT, NULL, evtcb_CWM_CMD_I2C1_LISTEN);

    /*##-1- Configure the I2C peripheral ######################################*/
    memI2C1.handle.Instance              = I2Cx;
    memI2C1.handle.Init.Timing           = I2C_TIMING;
    memI2C1.handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    memI2C1.handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    memI2C1.handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    memI2C1.handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;  
    memI2C1.handle.Init.OwnAddress1      = I2C_ADDRESS;
    memI2C1.handle.Init.OwnAddress2      = 0xFF;

    CWM_I2C1_DEINIT();

    if(HAL_I2C_Init(&memI2C1.handle) != HAL_OK)
    {
    /* Initialization Error */
    Error_Handler();
    }

    /* Enable the Analog I2C Filter */
    HAL_I2CEx_ConfigAnalogFilter(&memI2C1.handle,I2C_ANALOGFILTER_ENABLE);
    
}


static void I2cTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C1)
    {
        CWM_INTERNAL_CMD_SET(CWM_CMD_I2C1_TX_CPLT);
    }
}

static void I2cErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C1)
    {
        CWM_I2C1_DEINIT();
        CWM_I2C1_INIT();
    }
}


static I2cClass_t FunPointerI2C1 = {
    .handle = &memI2C1.handle,
    .init = CWM_I2C1_INIT,
    .deInit= CWM_I2C1_DEINIT,
    .mspInit= HAL_I2C1_MspInit,
    .mspDeInit= HAL_I2C1_MspDeInit,
    .TxCpltCallback= I2cTxCpltCallback,
    .ErrorCallback= I2cErrorCallback,
    .Write= I2cWrite,
    .Read= I2cRead,
    .buffWrite= I2cBuffWrite,
    .buffRead= I2cBuffRead,
    .singleRegWrite= I2cSingleRegWrite,
    .singleRegRead= I2cSingleRegRead,
};

pI2cClass_t CWM_GET_I2C1_ENTRY(void)
{
    return &FunPointerI2C1;
}

