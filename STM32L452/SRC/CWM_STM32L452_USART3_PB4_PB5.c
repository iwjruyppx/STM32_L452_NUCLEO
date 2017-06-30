
#include "CWM_STM32L452_USART3_PB4_PB5.h"

#ifdef USE_USART3_PB4_PB5
//#define USE_DMA
#define USE_IT

static UART_HandleTypeDef huart3;

#ifdef USE_DMA
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;
#endif /*USE_DMA*/ 

void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);

int CWM_USART3_WRITE(uint8_t * TxBuffer, int TxBufferSize)
{
#ifdef USE_DMA
    return HAL_UART_Transmit_DMA(&huart3, TxBuffer, TxBufferSize);
#elif defined ( USE_IT )
    return HAL_UART_Transmit_IT(&huart3, TxBuffer, TxBufferSize);
#else
    return HAL_UART_Transmit(&huart3, TxBuffer, TxBufferSize, 2*TxBufferSize);
#endif /*USE_IT*/ 
}

int CWM_USART3_READ(uint8_t * RxBuffer, int RxBufferSize)
{
#ifdef USE_DMA
    return HAL_UART_Receive_DMA(&huart3, RxBuffer, RxBufferSize);
#elif defined ( USE_IT )
    return HAL_UART_Receive_IT(&huart3, RxBuffer, RxBufferSize);
#else 
    return HAL_UART_Receive(&huart3, RxBuffer, RxBufferSize, 2*RxBufferSize);
#endif /*USE_IT*/ 
}

void USART3_IRQHandler(void);

/**
* @brief This function handles DMA1 channel2 global interrupt.
*/

void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart3);
}

#ifdef USE_DMA
void DMA1_Channel2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
}

/**
* @brief This function handles DMA1 channel3 global interrupt.
*/
void DMA1_Channel3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
}

/**
* @brief This function handles USART3 global interrupt.
*/
#endif /*USE_DMA*/ 
void HAL_UART_MspInit_USART3_PB4_PB5(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;
    
    if(huart->Instance == USART3)
    {
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
        PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
        {
            Error_Handler();
        }
        /* GPIO Ports Clock Enable */
        __HAL_RCC_GPIOC_CLK_ENABLE();

        /* USER CODE BEGIN USART3_MspInit 0 */
        /* USER CODE END USART3_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();


        /**USART3 GPIO Configuration    
        PC4     ------> USART3_TX
        PC5     ------> USART3_RX 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin = GPIO_PIN_8;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
        
        /* USART3 interrupt Init */
        HAL_NVIC_SetPriority(USART3_IRQn, 8, 2);
        HAL_NVIC_EnableIRQ(USART3_IRQn);

#ifdef USE_DMA
        /* DMA controller clock enable */
        __HAL_RCC_DMA1_CLK_ENABLE();
        /* USART3 DMA Init */
        /* USART3_RX Init */
        hdma_usart3_rx.Instance = DMA1_Channel3;
        hdma_usart3_rx.Init.Request = DMA_REQUEST_2;
        hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart3_rx.Init.Mode = DMA_NORMAL;
        hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(huart,hdmarx,hdma_usart3_rx);

        /* USART3_TX Init */
        hdma_usart3_tx.Instance = DMA1_Channel2;
        hdma_usart3_tx.Init.Request = DMA_REQUEST_2;
        hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart3_tx.Init.Mode = DMA_NORMAL;
        hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(huart,hdmatx,hdma_usart3_tx);

        /* DMA interrupt init */
        /* DMA1_Channel2_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 9, 3);
        HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
        /* DMA1_Channel3_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 9, 2);
        HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
#endif /*USE_DMA*/ 
    }
}
void HAL_UART_MspDeInit_USART3_PB4_PB5(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART3)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /**USART3 GPIO Configuration    
        PC4     ------> USART3_TX
        PC5     ------> USART3_RX 
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4|GPIO_PIN_5);

#ifdef USE_DMA
        /* USART3 DMA DeInit */
        HAL_DMA_DeInit(huart->hdmarx);
        HAL_DMA_DeInit(huart->hdmatx);
#endif /*USE_DMA*/ 

        /* USART3 interrupt DeInit */
        HAL_NVIC_DisableIRQ(USART3_IRQn);
    }

}

UART_HandleTypeDef *CWM_USART3_GET_HANDLE(void)
{
    return &huart3;
}

void CWM_UART_INIT_USART3_PB4_PB5(void)
{
    /*BaudRate = 9600; transmit data speed about 1000byte/s*/

    huart3.Instance = USART3;
    huart3.Init.BaudRate = 9600;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
    }
}

#endif
