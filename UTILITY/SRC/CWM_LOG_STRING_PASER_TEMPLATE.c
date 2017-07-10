
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "CWM_LOG_STRING_PASER_TEMPLATE.h"

#ifdef USE_LOG_STRING_PASSER_TEMPLATE

/*
    Example:
    
    LogStrPaserHandle_t mUart3Cmd;
    
    #define WriteUart3Msg( fmt, args...)            mUart3Cmd.LOGE(&mUart3Cmd, "AT"fmt, ##args)

    CWM_LOG_STRING_PASER_INIT(&mUart3Cmd);
    mUart3Cmd.LOGE_CallBack = CWM_USART3_WRITE;

    WriteUart3Msg("+VERSION");

#define BT_SEND(fmt, args...) \
    if(NULL != BLE_HC06S.sendData)\
        BLE_HC06S.sendData(&BLE_HC06S, fmt, ##args)
*/


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

/******************************************************************************/
/******************************************************************************/
/*Utility*/
/******************************************************************************/

/*

    input: 
        stringdata: paser data;
        string: paser identifier.
    output:
        < 0 : fail, string not found.
        >=0: count of stringdata point.


*/
int CWM_STRING_COMPARE(char *stringdata , char * string)
{
    int count = 0;
    char * ptrd ;
    char * ptrs;
    while(NULL != stringdata[count])
    {
        if(stringdata[count] == string[0]){
            ptrd = &stringdata[count];
            ptrs = string;
            do{
                if(ptrd[0] != ptrs[0])
                    break;
                
                ptrd++;
                ptrs ++;
                if(NULL == ptrs)
                    return count;
                if(0x00 == ptrs[0])
                    return count;
                if(NULL == ptrd)
                    return CWM_ERROR_NOT_FOUND;
            }while(1);
        }
        count++;
    }
    return CWM_ERROR_NOT_FOUND;
}

/*
    double to string convert.
*/
const char* doubleToString(double value)
{
    static char temp[16];
    char temp1[16];
    int zero_check = 1;
    int len;
    int i;
    sprintf(temp1,"%d",(int)((value -((int)value))*1000000000));
    
    len = strlen(temp1);
    if(len < 9)
    {
        for(i=0;i<=(len);i++)
        {
            temp1[9-i] =  temp1[len-i];
            if(zero_check)
            {
                if (temp1[9-i] == 48 || temp1[9-i]==0) 
                    temp1[9-i] = 0;
                else
                    zero_check = 0;
            }
        }
        for(i=0;i<(9-len);i++)
        {
            temp1[i] =  '0';
        }
    }
    else
    {
        for(i=0;i<=(len);i++)
        {
            if(zero_check)
            {
                if (temp1[9-i] == 48 || temp1[9-i]==0) 
                    temp1[9-i] = 0;
                else
                    zero_check = 0;
            }
        }
    }
    
    sprintf(temp,"%d.%s",(int)value,temp1);
    return (const char *)temp;
}

/******************************************************************************/
/*END Utility*/
/******************************************************************************/
/******************************************************************************/

#endif /*USE_LOG_STRING_PASSER_TEMPLATE*/
