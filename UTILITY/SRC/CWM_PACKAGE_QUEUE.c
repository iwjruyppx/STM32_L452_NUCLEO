
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "CWM_PACKAGE_QUEUE.h"

#ifdef USE_CWM_PACKAGE_QUEUE

/*Header[0]:count*/
#define PACKAGE_HEADER_SIZE    1

static int GetEmptySize(pPackageQueue_t ptr)
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

static int GetUseSize(pPackageQueue_t ptr)
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

static int PACKAGE_QUEUE_INIT(pPackageQueue_t mem, int size)
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

/*data format: byte[0] = count, byte[1~1+count] = data*/
static int PACKAGE_QUEUE_SET(pPackageQueue_t mem, uint8_t *data, int size)
{
    int i;
    if(GetEmptySize(mem) <= (size + PACKAGE_HEADER_SIZE))
        return CWM_ERROR_QUEUE_FULL;

    mem->mem[mem->front] = size;
    mem->front = (mem->front+1)%mem->Length;
        
    for(i =0;i<size;i++ )
    {
        mem->mem[mem->front] = data[i];
        mem->front = (mem->front+1)%mem->Length;
    }
    return CWM_NON;
}

/*data format: byte[0] = count, byte[1~1+count] = data*/
static int PACKAGE_QUEUE_GET(pPackageQueue_t mem, uint8_t *data)
{
    int i;
    int size;
    int count;
    size = GetUseSize(mem);
    if(0 == size)
        return CWM_NON;
    
    /*Read first byte to get this package size*/
    count = mem->mem[mem->rear];
    mem->rear = (mem->rear+1)%mem->Length;
    
    for(i =0;i<count;i++ )
    {
        data[i] = mem->mem[mem->rear];
        mem->rear = (mem->rear+1)%mem->Length;
    }
    return count;
}

static int PACKAGE_QUEUE_GET_COUNT(pPackageQueue_t mem)
{
    return GetUseSize(mem);
}

static PackageQueueClass_t FP_PACKAGE_QUEUE = {
    .init = PACKAGE_QUEUE_INIT,
    .set= PACKAGE_QUEUE_SET,
    .get= PACKAGE_QUEUE_GET,
    .getCount= PACKAGE_QUEUE_GET_COUNT,
};

pPackageQueueClass_t CWM_GET_PACKAGE_QUEUE_HANDLE(void)
{
    return &FP_PACKAGE_QUEUE;
}

/**********End Special Queue*/
/******************************************/

#endif /*USE_CWM_QUEUE*/
