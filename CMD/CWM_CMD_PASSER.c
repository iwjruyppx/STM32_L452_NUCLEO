
#include "CWM_CMD_PASSER.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_SCREEN.h"

void MsgPasser(pCWM_CMD_t cmd)
{
    switch(cmd->cmd){
        case CWM_CMD_UART_LISTEN_RX_UPDATE:
            CWM_GPS_DATA_PASSER_PROCESS();
            break;
        default:
            break;
    }
}

void CmdPasser(pCWM_CMD_t cmd)
{
    switch(cmd->cmd){
        case CWM_CMD_SCREEN_INIT:
        case CWM_CMD_SCREEN_ON:
        case CWM_CMD_SCREEN_OFF:
        case CWM_CMD_SCREEN_UPDATE:
        case CWM_CMD_SCREEN_CLEAN:
            CWM_SCREEN_CMD(cmd->cmd, NULL);
            break;
        case CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE:
            CWM_SCREEN_WRITE_AUTO_NEW_LINE(cmd->string);
            break;
        case CWM_CMD_UART_LISTEN:
            CWM_UART_LISTEN(64, NULL);
            break;
        case CWM_CMD_UART_LISTEN_DISABLE:
            //TODO
            break;
        case CWM_CMD_UART_LISTEN_RX_UPDATE:
            
            break;
        default:
            break;
    }
}
