
#ifndef __CWM_LOG_STRING_PASER_TEMPLATE_H
#define __CWM_LOG_STRING_PASER_TEMPLATE_H

#ifdef USE_LOG_STRING_PASSER_TEMPLATE

typedef struct LogStrPaserH_t{
    int (*LOGE)(struct LogStrPaserH_t *, const char * ,...);
    /*User need to implement this API*/
    int (*LOGE_CallBack)(uint8_t *,int);
    /*User need to implement this API End*/
} LogStrPaserHandle_t, *pLogStrPaserHandle_t;

void CWM_LOG_STRING_PASER_INIT(pLogStrPaserHandle_t Handle);


/******************************************************************************/
/******************************************************************************/
/*Utility*/
/******************************************************************************/
int CWM_STRING_COMPARE(char *stringdata , char * string);
const char* doubleToString(double value);
/******************************************************************************/
/*END Utility*/
/******************************************************************************/
/******************************************************************************/

#endif /*USE_LOG_STRING_PASSER_TEMPLATE*/
#endif /* __CWM_LOG_STRING_PASER_TEMPLATE_H */
