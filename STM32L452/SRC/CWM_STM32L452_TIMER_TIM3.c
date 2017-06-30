
#include "CWM_STM32L452_TIMER_TIM3.h"

#ifdef USE_TIMER3

TIM_HandleTypeDef htim3;

void TIM3_IRQHandler(void);

static void CWM_TIM3_SET_COUNT(uint32_t count)
{
    __HAL_TIM_SET_COUNTER(&htim3 , count);
}
static uint32_t CWM_TIM3_GET_COUNT(void)
{
    return __HAL_TIM_GET_COUNTER(&htim3);
}
static void CWM_TIM3_RESET_COUNT(void)
{
    CWM_TIM3_SET_COUNT(0);
}

static void CWM_TIM3_START(void)
{
    HAL_TIM_Base_Start_IT(&htim3);
}
static void CWM_TIM3_STOP(void)
{
    HAL_TIM_Base_Stop_IT(&htim3);
}

/**
* @brief This function handles TIM3 global interrupt.
*/
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim3);
}

static void CWM_TIM3_SET_PERIOD(uint32_t Period)
{
    htim3.Init.Period = Period;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
        Error_Handler();
}

static void HAL_TIM3_Base_MspInit(void* init)
{
    TIM_HandleTypeDef* htim_base = (TIM_HandleTypeDef*) init;
    if(htim_base->Instance==TIM3)
    {
        /* Peripheral clock enable */
        __HAL_RCC_TIM3_CLK_ENABLE();
        /* TIM3 interrupt Init */
        HAL_NVIC_SetPriority(TIM3_IRQn, 9, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
}

static void HAL_TIM3_Base_MspDeInit(void* init)
{
    TIM_HandleTypeDef* htim_base = (TIM_HandleTypeDef*) init;
    if(htim_base->Instance==TIM3)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM3_CLK_DISABLE();
        /* TIM3 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM3_IRQn);
    }
}

static void CWM_TIM3_INIT(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = (uint32_t)((SystemCoreClock /1000) - 1);
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 1000;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
        Error_Handler();

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
        Error_Handler();

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
        Error_Handler();

}

static void CWM_TIM3_DEINIT(void)
{
    if (HAL_TIM_Base_DeInit(&htim3) != HAL_OK)
        Error_Handler();
}



static TimerClass_t FunPointerTime3 = {
    .init = CWM_TIM3_INIT,
    .deInit = CWM_TIM3_DEINIT,
    .mspInit = HAL_TIM3_Base_MspInit,
    .mspDeInit = HAL_TIM3_Base_MspDeInit,
    .start = CWM_TIM3_START,
    .stop = CWM_TIM3_STOP,
    .setCount = CWM_TIM3_SET_COUNT,
    .getCount = CWM_TIM3_GET_COUNT,
    .resetCount = CWM_TIM3_RESET_COUNT,
    .setPeriod = CWM_TIM3_SET_PERIOD,
};

pTimerClass_t CWM_GET_TIMER3_ENTRY(void)
{
    return (pTimerClass_t)&FunPointerTime3;
}

#endif  //USE_TIMER3
