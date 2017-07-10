
#ifndef __CWM_STM32L452_TIMER_H
#define __CWM_STM32L452_TIMER_H

typedef enum {
    CWM_TIMER3 = 0,
    CWM_TIMER_MAX,
}CWM_TIMER_e;

void CWM_TIMER_INIT(void);
void CWM_TIMER_REG_CALL_BACK(CWM_TIMER_e timerX, CWM_CALLBACK pcallBack);
pTimerClass_t CWM_GET_TIMER_ENTRY(CWM_TIMER_e timerX);

#endif /* __CWM_STM32L452_I2C_H */
