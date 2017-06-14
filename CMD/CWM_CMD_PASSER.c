
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "CWM_CMD_PASSER.h"

#include "CWM_SCREEN.h"

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
        default:
            break;
    }
}
