#include "CwmQueue.h"

#ifdef USE_CWM_QUEUE

__weak void IRQ_ENABLE(void);
__weak void IRQ_DISABLE(void);

__weak void IRQ_ENABLE(void)
{
    __enable_interrupt();
}
__weak void IRQ_DISABLE(void)
{
    __disable_interrupt();
    __DSB();
    __ISB();
}

int CwmQueueGetEmptySize(pCwmQueue_t ptr)
{
    int EmptySize = 0;
    int front = ptr->front;
    int rear = ptr->rear;

    IRQ_DISABLE();
    if((front == rear) ||(front > rear))
        EmptySize = ptr->Length -(front - rear);
    else
        EmptySize = rear -front;
    IRQ_ENABLE();

    return EmptySize;
}

int CwmQueueGetEmptySizeFromISR(pCwmQueue_t ptr)
{
    int EmptySize = 0;
    int front = ptr->front;
    int rear = ptr->rear;

    if((front == rear) ||(front > rear))
        EmptySize = ptr->Length -(front - rear);
    else
        EmptySize = rear -front;

    return EmptySize;
}

int CwmQueueGetUseSize(pCwmQueue_t ptr)
{
    int size = 0;
    int front = ptr->front;
    int rear = ptr->rear;

    IRQ_DISABLE();
    if((front == rear) ||(front > rear))
        size = front - rear;
    else
        size = ptr->Length -(rear - front);
    IRQ_ENABLE();
    
    return size;
}

int CwmQueueGetUseSizeFromISR(pCwmQueue_t ptr)
{
    int size = 0;
    int front = ptr->front;
    int rear = ptr->rear;

    if((front == rear) ||(front > rear))
        size = front - rear;
    else
        size = ptr->Length -(rear - front);
    
    return size;
}

int CwmQueueSendToBack(pCwmQueue_t ptr,void *data)
{
	if(ptr == NULL)
		return CwmQueueError;

	IRQ_DISABLE();

	if(CwmQueueGetEmptySizeFromISR(ptr) ==0)
	{
		IRQ_ENABLE();

		return CwmQueueFull;
	}
	memcpy(&ptr->mem[ptr->front*ptr->ItemSize],data,ptr->ItemSize);
	ptr->front = (ptr->front+1)%ptr->Length;
	IRQ_ENABLE();

	return CwmQueueSuccess;
}

int CwmQueueSendToBackFromISR(pCwmQueue_t ptr,void *data)
{
	if(ptr == NULL)
		return CwmQueueError;

	if(CwmQueueGetEmptySizeFromISR(ptr) ==0)
        return CwmQueueFull;

	memcpy(&ptr->mem[ptr->front*ptr->ItemSize],data,ptr->ItemSize);
	ptr->front = (ptr->front+1)%ptr->Length;

	return CwmQueueSuccess;
}

int CwmQueueReceive(pCwmQueue_t ptr,void *data)
{
	if(ptr == NULL)
		return CwmQueueError;

	IRQ_DISABLE();

	if(CwmQueueGetUseSizeFromISR(ptr) ==0)
	{
		IRQ_ENABLE();

		return CwmQueueEmpty;
	}
	memcpy(data,&ptr->mem[ptr->rear*ptr->ItemSize],ptr->ItemSize);
	ptr->rear = (ptr->rear+1)%ptr->Length;
	IRQ_ENABLE();

	return CwmQueueSuccess;
}

int CwmQueueReceiveFromISR(pCwmQueue_t ptr,void *data)
{
	if(ptr == NULL)
		return CwmQueueError;

	if(CwmQueueGetUseSizeFromISR(ptr) ==0)
		return CwmQueueEmpty;
    
	memcpy(data,&ptr->mem[ptr->rear*ptr->ItemSize],ptr->ItemSize);
	ptr->rear = (ptr->rear+1)%ptr->Length;

	return CwmQueueSuccess;
}

int CwmQueueReset(pCwmQueue_t ptr)
{
	if(ptr == NULL)
		return CwmQueueError;

	ptr->front = 0;
	ptr->rear = 0;

	return CwmQueueSuccess;
}

#endif /*USE_CWM_QUEUE*/
