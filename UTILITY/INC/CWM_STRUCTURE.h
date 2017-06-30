
#ifndef __CWM_STRUCTURE_H
#define __CWM_STRUCTURE_H

typedef void *(*CWM_MALLOC)(size_t size);
typedef void (*CWM_FREE)(void *);

typedef void (*CWM_CALLBACK)(void *);
typedef void (*CWM_EVT_CALLBACK)(void *, void *);
typedef int(*CWM_INFO_CALLBACK)(int ,void *);
typedef int(*CWM_STRING_CALLBACK)(uint8_t *,int);

#define MAX_CWM_CMD_DATA_SIZE 32

typedef struct {
    int cmd;
    union {
        struct  {
            float data[MAX_CWM_CMD_DATA_SIZE/4];
            };
        struct  {
            char string[MAX_CWM_CMD_DATA_SIZE];
            };
        struct  {
            int type;
            int device;
            };
    };
}CWM_CMD_t, *pCWM_CMD_t;

typedef struct {
    void (*init)(void);
    void (*deInit)(void);
    void (*mspInit)(void *);
    void (*mspDeInit)(void *);
    void (*start)(void);
    void (*stop)(void);
    void (*irqCallBack)(void *);
    
    void (*setCount)(uint32_t);
    uint32_t (*getCount)(void);
    void (*resetCount)(void);
    void (*setPeriod)(uint32_t);
} TimerClass_t, *pTimerClass_t;

#endif /* __CWM_STRUCTURE_H */
