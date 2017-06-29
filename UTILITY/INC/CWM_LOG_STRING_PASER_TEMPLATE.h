
#ifndef __CWM_LOG_STRING_PASER_TEMPLATE_H
#define __CWM_LOG_STRING_PASER_TEMPLATE_H

#include "CWM_UTILITY.h"

typedef struct LogStrPaserH_t{
    int (*LOGE)(struct LogStrPaserH_t *, const char * ,...);
    /*User need to implement this API*/
    CWM_STRING_CALLBACK LOGE_CallBack;
    /*User need to implement this API End*/
} LogStrPaserHandle_t, *pLogStrPaserHandle_t;

void CWM_LOG_STRING_PASER_INIT(pLogStrPaserHandle_t Handle);

#endif /* __CWM_LOG_STRING_PASER_TEMPLATE_H */
