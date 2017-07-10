
#ifndef __CWM_STM32L452_UART_H
#define __CWM_STM32L452_UART_H

typedef enum {
    CWM_USART1 = 0,
    CWM_USART2,
    CWM_USART3,
    CWM_UART4,
    CWM_USART_MAX,
}CWM_UART_e;

typedef struct {
    UART_HandleTypeDef *handle;
    
    void (*init)(void);
    void (*deInit)(void);
    void (*mspInit)(UART_HandleTypeDef *);
    void (*mspDeInit)(UART_HandleTypeDef *);

    int (*write)(uint8_t *, int);
    int (*read)(uint8_t *, int);
} UsartClass_t, *pUsartClass_t;

void CWM_UART_INIT(void);

pUsartClass_t CWM_GET_USART_HANDLE(CWM_UART_e dev);
int GET_GPS_UART_STRING(void);

#endif /* __CWM_STM32L452_I2C_H */
