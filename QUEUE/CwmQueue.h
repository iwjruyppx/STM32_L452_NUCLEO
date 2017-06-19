#ifndef __CwmQueue_H__
#define __CwmQueue_H__

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef struct QueueDefinition
{
volatile int front;
volatile int rear;
    int Length;     //total queue leng
    int ItemSize;   // queue data size
    uint8_t *mem;
}CwmQueue_t, *pCwmQueue_t;

#define CwmQueueError -3
#define CwmQueueFull -2
#define CwmQueueEmpty -1
#define CwmQueueSuccess 0

int CwmQueueGetEmptySize(pCwmQueue_t ptr);
int CwmQueueGetEmptySizeFromISR(pCwmQueue_t ptr);
int CwmQueueGetUseSize(pCwmQueue_t ptr);
int CwmQueueGetUseSizeFromISR(pCwmQueue_t ptr);
int CwmQueueSendToBack(pCwmQueue_t ptr,void *data);
int CwmQueueSendToBackFromISR(pCwmQueue_t ptr,void *data);
int CwmQueueReceive(pCwmQueue_t ptr,void *data);
int CwmQueueReceiveFromISR(pCwmQueue_t ptr,void *data);
int CwmQueueReset(pCwmQueue_t ptr);

#endif /* __CwmQueue_H__ */
