
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "CWM_UART_QUEUE.h"

#define CWM_UART_QUEUE_BUFF_SIZE    1024

int CWM_UART_QUEUE_INIT(pCwmQueue_t mem, int size)
{
    mem->front = 0;
    mem->rear = 0;
    mem->Length = CWM_UART_QUEUE_BUFF_SIZE;
    mem->mem = pvPortMalloc(size);
    if(NULL == mem->mem)
        return CWM_ERROR_MEMORY_ALLOC_FAIL;
    
    memset(mem->mem, 0x00, size);
    return CWM_NON;
}

int CWM_UART_QUEUE_SET(pCwmQueue_t mem, uint8_t *data, int size)
{
    if(CwmQueueGetEmptySizeFromISR(mem) <= size)
        return CwmQueueFull;
    
    memcpy(&mem->mem[mem->front],data,size);
    mem->front = (mem->front+size)%mem->Length;
    return CwmQueueSuccess;
}
int CWM_UART_QUEUE_GET(pCwmQueue_t mem, uint8_t *data, int size)
{
    if(CwmQueueGetUseSizeFromISR(mem) < size)
		return CwmQueueError;
    
    memcpy(data,&mem->mem[mem->rear],size);
    mem->rear = (mem->rear+size)%mem->Length;
    return CwmQueueSuccess;
}
int CWM_UART_QUEUE_GET_COUNT(pCwmQueue_t mem)
{
    return CwmQueueGetEmptySizeFromISR(mem);
}
