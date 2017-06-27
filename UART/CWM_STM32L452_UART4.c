
#include "CWM_STM32L452_UART4.h"

#ifdef USE_UART4_PA0_PA1
/***********************************************************/
/**********************//*CONFIG*/
/***********************************************************/
/* User can use this section to tailor CWM_UART4/UARTx instance used and associated 
   resources */
/* Definition for CWM_UART4 clock resources */
#define CWM_UART4                           UART4
#define CWM_UART4_CLK_ENABLE()              __HAL_RCC_UART4_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA2_CLK_ENABLE()
#define CWM_UART4_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define CWM_UART4_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define CWM_UART4_FORCE_RESET()             __HAL_RCC_UART4_FORCE_RESET()
#define CWM_UART4_RELEASE_RESET()           __HAL_RCC_UART4_RELEASE_RESET()

/* Definition for CWM_UART4 Pins */
#define CWM_UART4_TX_PIN                    GPIO_PIN_0
#define CWM_UART4_TX_GPIO_PORT              GPIOA
#define CWM_UART4_TX_AF                     GPIO_AF8_UART4
#define CWM_UART4_RX_PIN                    GPIO_PIN_1
#define CWM_UART4_RX_GPIO_PORT              GPIOA
#define CWM_UART4_RX_AF                     GPIO_AF8_UART4

/* Definition for CWM_UART4's DMA */
#define CWM_UART4_TX_DMA_CHANNEL             DMA2_Channel3
#define CWM_UART4_RX_DMA_CHANNEL             DMA2_Channel5

/* Definition for CWM_UART4's DMA Request */
#define CWM_UART4_TX_DMA_REQUEST             DMA_REQUEST_2
#define CWM_UART4_RX_DMA_REQUEST             DMA_REQUEST_2

/* Definition for CWM_UART4's NVIC */
#define CWM_UART4_DMA_TX_IRQn                DMA2_Channel3_IRQn
#define CWM_UART4_DMA_RX_IRQn                DMA2_Channel5_IRQn
#define CWM_UART4_DMA_TX_IRQHandler          DMA2_Channel3_IRQHandler
#define CWM_UART4_DMA_RX_IRQHandler          DMA2_Channel5_IRQHandler

/* Definition for CWM_UART4's NVIC */
#define CWM_UART4_IRQn                      UART4_IRQn
#define CWM_UART4_IRQHandler                UART4_IRQHandler
/***********************************************************/
/**********************//*CONFIG*/
/***********************************************************/

void CWM_UART4_DMA_RX_IRQHandler(void);
void CWM_UART4_DMA_TX_IRQHandler(void);
void CWM_UART4_IRQHandler(void);

UART_HandleTypeDef CWM_UART4_Handle;

int CWM_UART4_WRITE(uint8_t * TxBuffer, int TxBufferSize)
{
    return HAL_UART_Transmit_DMA(&CWM_UART4_Handle, TxBuffer, TxBufferSize);
}

int CWM_UART4_READ(uint8_t * RxBuffer, int RxBufferSize)
{
    return HAL_UART_Receive_DMA(&CWM_UART4_Handle, RxBuffer, RxBufferSize);
}

/**
  * @brief  This function handles DMA interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA  
  *         used for USART data transmission     
  */
void CWM_UART4_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(CWM_UART4_Handle.hdmarx);
}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA  
  *         used for USART data reception    
  */
void CWM_UART4_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(CWM_UART4_Handle.hdmatx);
}


/**
  * @brief  This function handles UART interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA  
  *         used for USART data transmission     
  */
void CWM_UART4_IRQHandler(void)
{
  HAL_UART_IRQHandler(&CWM_UART4_Handle);
}

/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit_UART4_PA0_PA1(UART_HandleTypeDef *huart)
{
  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;
  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  CWM_UART4_TX_GPIO_CLK_ENABLE();
  CWM_UART4_RX_GPIO_CLK_ENABLE();


  /* Enable USARTx clock */
  CWM_UART4_CLK_ENABLE();

  /* Enable DMA clock */
  DMAx_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = CWM_UART4_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = CWM_UART4_TX_AF;

  HAL_GPIO_Init(CWM_UART4_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = CWM_UART4_RX_PIN;
  GPIO_InitStruct.Alternate = CWM_UART4_RX_AF;

  HAL_GPIO_Init(CWM_UART4_RX_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the DMA ##################################################*/
  /* Configure the DMA handler for Transmission process */
  hdma_tx.Instance                 = CWM_UART4_TX_DMA_CHANNEL;
  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_tx.Init.Mode                = DMA_NORMAL;
  hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
  hdma_tx.Init.Request             = CWM_UART4_TX_DMA_REQUEST;

  HAL_DMA_Init(&hdma_tx);

  /* Associate the initialized DMA handle to the UART handle */
  __HAL_LINKDMA(huart, hdmatx, hdma_tx);

  /* Configure the DMA handler for reception process */
  hdma_rx.Instance                 = CWM_UART4_RX_DMA_CHANNEL;
  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_rx.Init.Mode                = DMA_NORMAL;
  hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_rx.Init.Request             = CWM_UART4_RX_DMA_REQUEST;

  HAL_DMA_Init(&hdma_rx);

  /* Associate the initialized DMA handle to the the UART handle */
  __HAL_LINKDMA(huart, hdmarx, hdma_rx);
    
  /*##-4- Configure the NVIC for DMA #########################################*/

  /* by Paul
    FreeRTOS priority default setting is 80, if we setting  HAL_NVIC_SetPriority(CWM_UART4_DMA_RX_IRQn, 8, 0); 
    ucCurrentPriority == 128 (2^8)
    if we not set this parameter over than 80, system will block on void vPortValidateInterruptPriority( void ) function.
  */
  /* NVIC configuration for DMA transfer complete interrupt (UART4_TX) */
  HAL_NVIC_SetPriority(CWM_UART4_DMA_TX_IRQn, 8, 1);
  HAL_NVIC_EnableIRQ(CWM_UART4_DMA_TX_IRQn);
    
  /* NVIC configuration for DMA transfer complete interrupt (UART4_RX) */
  HAL_NVIC_SetPriority(CWM_UART4_DMA_RX_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(CWM_UART4_DMA_RX_IRQn);
  
  /* NVIC for USART, to catch the TX complete */
  HAL_NVIC_SetPriority(CWM_UART4_IRQn, 8, 1);
  HAL_NVIC_EnableIRQ(CWM_UART4_IRQn);
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */

void HAL_UART_MspDeInit_UART4_PA0_PA1(UART_HandleTypeDef *huart)
{
    
    /*##-1- Reset peripherals ##################################################*/
    CWM_UART4_FORCE_RESET();
    CWM_UART4_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks #################################*/
    /* Configure USARTx Tx as alternate function  */
    HAL_GPIO_DeInit(CWM_UART4_TX_GPIO_PORT, CWM_UART4_TX_PIN);
    /* Configure USARTx Rx as alternate function  */
    HAL_GPIO_DeInit(CWM_UART4_RX_GPIO_PORT, CWM_UART4_RX_PIN);

    /*##-3- Disable the DMA #####################################################*/
    /* De-Initialize the DMA channel associated to reception process */
    if(huart->hdmarx != 0)
    {
        HAL_DMA_DeInit(huart->hdmarx);
    }
    /* De-Initialize the DMA channel associated to transmission process */
    if(huart->hdmatx != 0)
    {
        HAL_DMA_DeInit(huart->hdmatx);
    }  

    /*##-4- Disable the NVIC for DMA ###########################################*/
    HAL_NVIC_DisableIRQ(CWM_UART4_DMA_TX_IRQn);
    HAL_NVIC_DisableIRQ(CWM_UART4_DMA_RX_IRQn);
}

void CWM_UART_INIT_UART4_PA0_PA1(void)
{

    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART configured as follows:
    - Word Length = 8 Bits
    - Stop Bit = One Stop bit
    - Parity = None
    - BaudRate = 9600 baud
    - Hardware flow control disabled (RTS and CTS signals) */
    CWM_UART4_Handle.Instance        = CWM_UART4;

    CWM_UART4_Handle.Init.BaudRate   = 9600;
    CWM_UART4_Handle.Init.WordLength = UART_WORDLENGTH_8B;
    CWM_UART4_Handle.Init.StopBits   = UART_STOPBITS_1;
    CWM_UART4_Handle.Init.Parity     = UART_PARITY_NONE;
    CWM_UART4_Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    CWM_UART4_Handle.Init.Mode       = UART_MODE_TX_RX;
    
    if(HAL_UART_DeInit(&CWM_UART4_Handle) != HAL_OK)
    {
        Error_Handler();
    }  
    if(HAL_UART_Init(&CWM_UART4_Handle) != HAL_OK)
    {
        Error_Handler();
    }

}

#endif
