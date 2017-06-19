
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "CWM_SCREEN.h"

#include "tm_stm32f4_ssd1306.h"
#include "tm_stm32f4_fonts.h"

static int SCREEN_CURRENT_STATE = CWM_CMD_FALL_SCREEN_NO_INITIAL;

int CWM_SCREEN_GET_STATE(void)
{
    return SCREEN_CURRENT_STATE;
}

int CWM_SCREEN_CMD(int cmd, void *p)
{
    pCWM_SCREEN_CMD_t cmd_data = (pCWM_SCREEN_CMD_t) p;
    
    switch(cmd)
    {
        case CWM_CMD_SCREEN_INIT:
            SCREEN_CURRENT_STATE = cmd;
            TM_SSD1306_Init();
            break;
        case CWM_CMD_SCREEN_ON:
            SCREEN_CURRENT_STATE = cmd;
            SSD1306_ON();
            break;
        case CWM_CMD_SCREEN_OFF:
            SCREEN_CURRENT_STATE = cmd;
            SSD1306_OFF();
            break;
        case CWM_CMD_SCREEN_CLEAN:
            TM_SSD1306_ScreenClean();
            break;
        case CWM_CMD_SCREEN_UPDATE:
            TM_SSD1306_UpdateScreen();
            break;
        case CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE:
            return CWM_SSD1306_Puts_Auto_newLine(cmd_data->string, &TM_Font_7x10, SSD1306_COLOR_WHITE);

        default:
            break;
    }
    return 0;
}

int CWM_SCREEN_WRITE_AUTO_NEW_LINE(char * data)
{
    CWM_SCREEN_CMD_t  screen_cmd;
    screen_cmd.string = data;
    
    return CWM_SCREEN_CMD(CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE, &screen_cmd);    
}
