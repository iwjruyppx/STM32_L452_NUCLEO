
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "MODULE_BT_HC_06_SLAVE.h"

#ifdef USE_MODULE_BT_HC_06_SLAVE
/*****************************************/
/*
AT代刚莱OK
AT+VERSION莱?砰セ
AT+NAMExyz盢杆竚嘿эxyzn                                  
AT+PIN1234盢硈絬盞絏传1234
AT+BAUD4盢 baud rate 传 9600
AT+BAUD5盢 baud rate 传 19200
AT+BAUD6盢 baud rate 传 38400
AT+BAUD7盢 baud rate 传 57600

AT+ROLE=1
AT+BIND=2016,5,232325
*/
/*****************************************/

static const char HC06_TEST[] = "AT";
static const char HC06_GET_VERSION[] = "AT+VERSION";
static const char HC06_SET_NAME[] = "AT+NAME";
static const char HC06_SET_PIN[] = "AT+PIN";
static const char HC06_SET_BAUD[] = "AT+BAUD";

static int WRITE_CMD_TO_CALLBACK(struct ModuleHc06Class *handle, const char * format,...){
    va_list argList;
    uint8_t tBuffer[255] ;
    int size;

    va_start(argList,format);
    size = vsnprintf((char *)tBuffer,sizeof(tBuffer),format,argList);
    va_end(argList);

    if(NULL == handle->stringWriteCallBack)
        return CWM_ERROR_NULL_POINT;
    
    return handle->stringWriteCallBack(tBuffer, size);
}

static int MODULE_HC_06_SET_CMD(struct ModuleHc06Class *handle, HC06_CMD_e cmd, uint8_t* string)
{  
    switch(cmd)
    {
        case CMD_HC06_TEST:
            return WRITE_CMD_TO_CALLBACK(handle, "%s", HC06_TEST);
        case CMD_HC06_GET_VERSION:
            return WRITE_CMD_TO_CALLBACK(handle, "%s", HC06_GET_VERSION);
        case CMD_HC06_SET_NAME:
            return WRITE_CMD_TO_CALLBACK(handle, "%s%s", HC06_SET_NAME, string);
        case CMD_HC06_SET_PIN:
            return WRITE_CMD_TO_CALLBACK(handle, "%s%s", HC06_SET_PIN, string);
        case CMD_HC06_SET_BAUD_1200:
            return WRITE_CMD_TO_CALLBACK(handle, "%s%d", HC06_SET_BAUD, 1);
        case CMD_HC06_SET_BAUD_2400:
            return WRITE_CMD_TO_CALLBACK(handle, "%s%d", HC06_SET_BAUD, 2);
        case CMD_HC06_SET_BAUD_4800:
            return WRITE_CMD_TO_CALLBACK(handle, "%s%d", HC06_SET_BAUD, 3);
        case CMD_HC06_SET_BAUD_9600:
            return WRITE_CMD_TO_CALLBACK(handle, "%s%d", HC06_SET_BAUD, 4);
        case CMD_HC06_SET_BAUD_19200:
            return WRITE_CMD_TO_CALLBACK(handle, "%s%d", HC06_SET_BAUD, 5);
        case CMD_HC06_SET_BAUD_38400:
            return WRITE_CMD_TO_CALLBACK(handle, "%s%d", HC06_SET_BAUD, 6);
        case CMD_HC06_SET_BAUD_57600:
            return WRITE_CMD_TO_CALLBACK(handle, "%s%d", HC06_SET_BAUD, 7);
        case CMD_HC06_SET_BAUD_115200:
            return WRITE_CMD_TO_CALLBACK(handle, "%s%d", HC06_SET_BAUD, 8);
        case CMD_HC06_SET_BAUD_230400:
            return WRITE_CMD_TO_CALLBACK(handle, "%s%d", HC06_SET_BAUD, 9);
        case CMD_HC06_SET_BAUD_460800:
            return WRITE_CMD_TO_CALLBACK(handle, "%sA", HC06_SET_BAUD);
        case CMD_HC06_SET_BAUD_921600:
            return WRITE_CMD_TO_CALLBACK(handle, "%sB", HC06_SET_BAUD);
        case CMD_HC06_SET_BAUD_1382400:
            return WRITE_CMD_TO_CALLBACK(handle, "%sC", HC06_SET_BAUD);
        default:
            return CWM_ERROR_CMD_NOT_FOUND;
    }
}

void MODULE_HC_06_INIT(pModuleHc06Class_t handle)
{
    handle->setCmd = MODULE_HC_06_SET_CMD;
    handle->sendData= WRITE_CMD_TO_CALLBACK;
}

#endif /*USE_MODULE_BT_HC_06_SLAVE*/
