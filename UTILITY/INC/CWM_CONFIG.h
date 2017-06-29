#ifndef __CWM_CONFIG_H
#define __CWM_CONFIG_H

/*Timer*/
#define USE_TIMER

#ifdef USE_TIMER
    #define USE_TIMER3
    //#define USE_TIMER_PWM_PULSE
#endif

/*I2C*/
#define USE_I2C

/*UART*/
#define USE_UART

/*USART*/
//#define USE_USART

#ifdef USE_I2C
    #define USE_I2C1_PB8_PB9
#endif

#ifdef USE_UART
//    #define USE_UART4_PA0_PA1
    #define USE_USART3_PB4_PB5
#endif
#ifdef USE_USART
#endif

#endif /* __CWM_CONFIG_H */
