
#ifndef __CWM_UART_QUEUE_H
#define __CWM_UART_QUEUE_H

#include "CWM_UTILITY.h"

#define CWM_UART_QUEUE_BUFF_SIZE    1024

void CWM_UART_QUEUE_INIT(void);

int CWM_UART_QUEUE_SET(uint8_t *data, int size);
int CWM_UART_QUEUE_GET(uint8_t *data, int size);
int CWM_UART_QUEUE_GET_COUNT(void);

#endif /* __CWM_UART_QUEUE_H */
