
#ifndef __CWM_UART_QUEUE_H
#define __CWM_UART_QUEUE_H

int CWM_UART_QUEUE_INIT(pCwmQueue_t mem, int size);

int CWM_UART_QUEUE_SET(pCwmQueue_t mem, uint8_t *data, int size);
int CWM_UART_QUEUE_GET(pCwmQueue_t mem, uint8_t *data, int size);
int CWM_UART_QUEUE_GET_COUNT(pCwmQueue_t mem);

#endif /* __CWM_UART_QUEUE_H */
