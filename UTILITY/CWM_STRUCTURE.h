
#ifndef __CWM_STRUCTURE_H
#define __CWM_STRUCTURE_H

typedef void *(*CWM_MALLOC)(size_t size);
typedef void (*CWM_FREE)(void *);

typedef int(*CWM_CALLBACK)(void *);
typedef void (*CWM_EVT_CALLBACK)(void *, void *);
typedef int(*CWM_INFO_CALLBACK)(int ,void *);
typedef int(*CWM_UART_CALLBACK)(uint8_t *,int);


typedef struct {
    int cmd;
    union {
        struct  {
            float data[3];
            };
        struct  {
            char string[32];
            };
    };
}CWM_CMD_t, *pCWM_CMD_t;


#endif /* __CWM_STRUCTURE_H */
