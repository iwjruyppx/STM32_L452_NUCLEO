#ifndef __CWM_PACKAGE_QUEUE_H__
#define __CWM_PACKAGE_QUEUE_H__

#ifdef USE_CWM_PACKAGE_QUEUE
typedef struct
{
    volatile int front;
    volatile int rear;
    int Length;     //total queue leng
    int ItemSize;   // queue data size
    uint8_t *mem;
}PackageQueue_t, *pPackageQueue_t;

typedef struct {
    /*
        input[0]: pPackageQueue_t 
        input[1]: memory alloc size
        return: Error message.
    */
    int (*init)(pPackageQueue_t, int);
    /*
        input[0]: pPackageQueue_t 
        input[1]: data 
        input[2]: size
        return: Error message.
    */
    int (*set)(pPackageQueue_t, uint8_t *, uint16_t);
    /*
        input[0]: pPackageQueue_t 
        output[0]: data 
        return[>=0]: Count.
        return[<0]: Error info.
    */
    int (*get)(pPackageQueue_t, uint8_t *);
    /*
        input[0]: pPackageQueue_t 
        return: Count.
    */
    int (*getCount)(pPackageQueue_t);
} PackageQueueClass_t, *pPackageQueueClass_t;

pPackageQueueClass_t CWM_GET_PACKAGE_QUEUE_HANDLE(void);

#endif /*USE_CWM_PACKAGE_QUEUE*/

#endif /* __CWM_PACKAGE_QUEUE_H__ */
