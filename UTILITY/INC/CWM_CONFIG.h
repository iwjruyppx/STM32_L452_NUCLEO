#ifndef __CWM_CONFIG_H
#define __CWM_CONFIG_H

/******************************************************************************/
/******************************************************************************/
/*PERIPHERAL*/
/******************************************************************************/
/*Timer*/
#define USE_TIMER
/*I2C*/
#define USE_I2C
/*UART*/
#define USE_UART
/*******************************************/
#ifdef USE_TIMER
    #define USE_TIMER3
    //#define USE_TIMER_PWM_PULSE
#endif/*USE_TIMER*/
#ifdef USE_I2C
    #define USE_I2C1_PB8_PB9
#endif/*USE_I2C*/
#ifdef USE_UART
    #define USE_UART4_PA0_PA1
    #define USE_USART3_PB4_PB5
#endif/*USE_UART*/
/******************************************************************************/
/*END PERIPHERAL*/
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
/*Utility and module*/
/******************************************************************************/
/*GPS_PASSER*/
#define USE_GPS_PASSER

/*LINK_LIST_TEMPLATE*/
#define USE_LINK_LIST_TEMPLATE

/*LOG_STRING_PASSER_TEMPLATE*/
#define USE_LOG_STRING_PASSER_TEMPLATE

/*CWM_QUEUE*/
#define USE_CWM_QUEUE

/*MODULE_BT_HC_06_SLAVE*/
#define USE_MODULE_BT_HC_06_SLAVE

/*USE_OLED_DRIVER_SSD1306*/
#define USE_OLED_DRIVER_SSD1306

/******************************************************************************/
/*Utility and module*/
/******************************************************************************/
/******************************************************************************/

#endif /* __CWM_CONFIG_H */
