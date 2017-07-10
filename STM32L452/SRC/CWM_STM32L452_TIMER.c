
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_STM32L452_TIMER.h"
#include "CWM_STM32L452_TIMER_TIM3.h"

#ifdef USE_TIMER

static pTimerClass_t pfunTim[CWM_TIMER_MAX];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM6)
        HAL_IncTick();
        
#ifdef USE_TIMER3
    if(htim->Instance==TIM3)
        if(NULL != pfunTim[CWM_TIMER3]->irqCallBack)
            pfunTim[CWM_TIMER3]->irqCallBack(htim);
        
#endif  //USE_TIMER3
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
#ifdef USE_TIMER3
    if(NULL != pfunTim[CWM_TIMER3]->mspInit)
        pfunTim[CWM_TIMER3]->mspInit(htim_base);
#endif  //USE_TIMER3
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
#ifdef USE_TIMER3
    if(NULL != pfunTim[CWM_TIMER3]->mspDeInit)
        pfunTim[CWM_TIMER3]->mspDeInit(htim_base);
#endif  //USE_TIMER3
}

void CWM_TIMER_REG_CALL_BACK(CWM_TIMER_e timerX, CWM_CALLBACK pcallBack)
{
    if(CWM_TIMER_MAX <= timerX)
        return;
    
    pfunTim[timerX]->irqCallBack = pcallBack;
}
pTimerClass_t CWM_GET_TIMER_ENTRY(CWM_TIMER_e timerX)
{
    if(CWM_TIMER_MAX <= timerX)
        return NULL;
    
    return pfunTim[timerX];
}

void CWM_TIMER_INIT(void)
{

#ifdef USE_TIMER3
    pfunTim[CWM_TIMER3] = CWM_GET_TIMER3_ENTRY();

    if(NULL != pfunTim[CWM_TIMER3]->init)
        pfunTim[CWM_TIMER3]->init();
    if(NULL != pfunTim[CWM_TIMER3]->init)
        pfunTim[CWM_TIMER3]->start();
    
#endif  //USE_TIMER3
}
#else

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM6)
        HAL_IncTick();
}

void CWM_TIMER_INIT(void)
{
}
#endif  //USE_TIMER
