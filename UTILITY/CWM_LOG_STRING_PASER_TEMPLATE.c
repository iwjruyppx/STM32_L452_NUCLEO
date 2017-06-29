
#include "CWM_LOG_STRING_PASER_TEMPLATE.h"

static int LOGE(struct LogStrPaserH_t *handle, const char * format,...){
    va_list argList;
    int size;
    uint8_t tBuffer[255] ;

    va_start(argList,format);
    size = vsnprintf((char *)tBuffer,sizeof(tBuffer),format,argList);
    va_end(argList);

    if(NULL != handle->LOGE_CallBack)
        handle->LOGE_CallBack(tBuffer, size);
    return 0;
}

void CWM_LOG_STRING_PASER_INIT(pLogStrPaserHandle_t Handle)
{
    Handle->LOGE = LOGE;
}
