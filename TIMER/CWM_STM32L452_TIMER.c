
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
#endif  //USE_TIMER_PWM_PULSE