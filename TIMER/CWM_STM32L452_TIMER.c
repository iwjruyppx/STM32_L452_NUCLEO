
#include "CWM_STM32L452_TIMER.h"

#ifdef USE_TIMER_PWM_PULSE
    TIM_HandleTypeDef    CWMTimHandle;

    /* Timer Output Compare Configuration Structure declaration */
    TIM_OC_InitTypeDef sConfig;

    /* Capture Compare buffer */
    uint32_t aCCValue_Buffer[3] = {0, 0, 0};

    /* Timer Period*/
    uint32_t uwTimerPeriod  = 0;

    void CWM_TIMER_INIT(void)
    {
      uwTimerPeriod = (uint32_t)((SystemCoreClock / 17570) - 1);
      /* Compute CCR1 value to generate a duty cycle at 75% */
      aCCValue_Buffer[0] = (uint32_t)(((uint32_t) 75 * (uwTimerPeriod - 1)) / 100);
      /* Compute CCR2 value to generate a duty cycle at 50% */
      aCCValue_Buffer[1] = (uint32_t)(((uint32_t) 50 * (uwTimerPeriod - 1)) / 100);
      /* Compute CCR3 value to generate a duty cycle at 25% */
      aCCValue_Buffer[2] = (uint32_t)(((uint32_t) 25 * (uwTimerPeriod - 1)) / 100);
      
      CWMTimHandle.Instance = TIMx;

      CWMTimHandle.Init.Period            = uwTimerPeriod;
      CWMTimHandle.Init.RepetitionCounter = 3;
      CWMTimHandle.Init.Prescaler         = 0;
      CWMTimHandle.Init.ClockDivision     = 0;
      CWMTimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
      if (HAL_TIM_PWM_Init(&CWMTimHandle) != HAL_OK)
      {
        /* Initialization Error */
      }

      /*##-2- Configure the PWM channel 3 ########################################*/
      sConfig.OCMode       = TIM_OCMODE_PWM1;
      sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
      sConfig.Pulse        = aCCValue_Buffer[1];
      sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
      sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
      sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
      sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
      if (HAL_TIM_PWM_ConfigChannel(&CWMTimHandle, &sConfig, TIM_CHANNEL_3) != HAL_OK)
      {
        /* Configuration Error */
      }

      /*##-3- Start PWM signal generation in DMA mode ############################*/
      if (HAL_TIM_PWM_Start_DMA(&CWMTimHandle, TIM_CHANNEL_3, aCCValue_Buffer, 3) != HAL_OK)
      {
        /* Starting Error */
      }

    }

    int time_dma3 = 0;

    void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
    {
        time_dma3++;

    }
    

/**
* @brief  This function handles DMA interrupt request.
* @param  None
* @retval None
*/

void TIMx_DMA_IRQHandler(void);
void TIMx_DMA_IRQHandler(void)
{
  HAL_DMA_IRQHandler(CWMTimHandle.hdma[TIM_DMA_ID_CC3]);
}


/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  * @param htim: TIM handle pointer
  * @retval None
  */

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  static DMA_HandleTypeDef  hdma_tim;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx clock enable */
  TIMx_CLK_ENABLE();

  /* Enable GPIO Channel3/3N Clocks */
  TIMx_CHANNEL3_GPIO_CLK_ENABLE();

  /* Enable DMA clock */
  DMA_TIMER_CLK_ENABLE();


  /* Configure TIM1_Channel3 in output, push-pull & alternate function mode */
  GPIO_InitStruct.Pin = GPIO_PIN_CHANNEL3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF_TIMx;
  HAL_GPIO_Init(TIMx_GPIO_CHANNEL3_PORT, &GPIO_InitStruct);


  /* Set the parameters to be configured */
  hdma_tim.Init.Request  = TIMx_CC3_DMA_REQUEST;
  hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tim.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD ;
  hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_WORD ;
  hdma_tim.Init.Mode = DMA_CIRCULAR;
  hdma_tim.Init.Priority = DMA_PRIORITY_HIGH;

  /* Set hdma_tim instance */
  hdma_tim.Instance = TIMx_CC3_DMA_INST;

  /* Link hdma_tim to hdma[TIM_DMA_ID_CC3] (channel3) */
  __HAL_LINKDMA(htim, hdma[TIM_DMA_ID_CC3], hdma_tim);

  /* Initialize TIMx DMA handle */
  HAL_DMA_Init(htim->hdma[TIM_DMA_ID_CC3]);

  /*##-2- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(TIMx_DMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIMx_DMA_IRQn);
}
#else

void CWM_TIMER_INIT(void)
{
}
#endif  //USE_TIMER_PWM_PULSE
