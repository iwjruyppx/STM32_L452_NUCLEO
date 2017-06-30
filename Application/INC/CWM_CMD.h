
#ifndef __CWM_CMD_H
#define __CWM_CMD_H

typedef enum {    
    CWM_CMD_FAIL_SEND_TO_QUEUE = -100,
    CWM_CMD_FALL_SCREEN_NO_INITIAL = -1,
    CWM_CMD_NON = 0,
    
    /*SCREEN CMD*/
    CWM_CMD_SCREEN_INIT,
    CWM_CMD_SCREEN_ON,
    CWM_CMD_SCREEN_OFF,
    CWM_CMD_SCREEN_UPDATE,
    CWM_CMD_SCREEN_CLEAN,
    CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE,
    
    /*UART USART CMD*/
    CWM_CMD_USART_ENABLE,
    CWM_CMD_USART_DISABLE,
    CWM_CMD_USART_LISTEN,
    CWM_CMD_USART_LISTEN_DISABLE,
    CWM_CMD_USART1_TX_COMPLETE,
    CWM_CMD_USART1_RX_UPDATE,
    CWM_CMD_USART2_TX_COMPLETE,
    CWM_CMD_USART2_RX_UPDATE,
    CWM_CMD_USART3_TX_COMPLETE,
    CWM_CMD_USART3_RX_UPDATE,
    CWM_CMD_UART4_TX_COMPLETE,
    CWM_CMD_UART4_RX_UPDATE,
    
}uCWM_CMD;

#endif /* __CWM_CMD_H */
