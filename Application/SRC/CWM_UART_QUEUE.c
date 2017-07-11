
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "CWM_UART_QUEUE.h"

int CWM_UART_QUEUE_INIT(pCwmQueue_t mem, int size)
{
    mem->front = 0;
    mem->rear = 0;
    mem->Length = size;
    mem->mem = pvPortMalloc(size);
    if(NULL == mem->mem)
        return CWM_ERROR_MEMORY_ALLOC_FAIL;
    
    memset(mem->mem, 0x00, size);
    return CWM_NON;
}

int CWM_UART_QUEUE_SET(pCwmQueue_t mem, uint8_t *data, int size)
{
    int i;
    if(CwmQueueGetEmptySizeFromISR(mem) <= size)
        return CWM_ERROR_QUEUE_FULL;
    
    for(i =0;i<size;i++ )
    {
        mem->mem[mem->front] = data[i];
        mem->front = (mem->front+1)%mem->Length;
    }
    return CWM_NON;
}
int CWM_UART_QUEUE_GET(pCwmQueue_t mem, uint8_t *data, int size)
{
    int i;
    if(CwmQueueGetUseSizeFromISR(mem) < size)
		return CWM_ERROR;
    for(i =0;i<size;i++ )
    {
        data[i] = mem->mem[mem->rear];
        mem->rear = (mem->rear+1)%mem->Length;
    }
    return CWM_NON;
}

int CWM_UART_QUEUE_GET_COUNT(pCwmQueue_t mem)
{
    return CwmQueueGetUseSizeFromISR(mem);
}
