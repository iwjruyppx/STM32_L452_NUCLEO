
#include "CWM_UART_QUEUE.h"

static CwmQueue_t mem;
static uint8_t UART_BUFF[CWM_UART_QUEUE_BUFF_SIZE];

void CWM_UART_QUEUE_INIT(void)
{
    mem.front = 0;
    mem.rear = 0;
    mem.Length = CWM_UART_QUEUE_BUFF_SIZE;
    mem.mem = UART_BUFF;
    memset(mem.mem, 0x00, CWM_UART_QUEUE_BUFF_SIZE);
}

int CWM_UART_QUEUE_SET(uint8_t *data, int size)
{
    if(CwmQueueGetEmptySizeFromISR(&mem) <= size)
        return CwmQueueFull;
    
    memcpy(&mem.mem[mem.front],data,size);
    mem.front = (mem.front+size)%mem.Length;
    return CwmQueueSuccess;
}
int CWM_UART_QUEUE_GET(uint8_t *data, int size)
{
    if(CwmQueueGetUseSizeFromISR(&mem) < size)
		return CwmQueueError;
    
    memcpy(data,&mem.mem[mem.rear],size);
    mem.rear = (mem.rear+size)%mem.Length;
    return CwmQueueSuccess;
}
int CWM_UART_QUEUE_GET_COUNT(void)
{
    return CwmQueueGetEmptySizeFromISR(&mem);
}
