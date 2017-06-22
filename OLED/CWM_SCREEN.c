
#include "CWM_SCREEN.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

static int SCREEN_CURRENT_STATE = CWM_CMD_FALL_SCREEN_NO_INITIAL;
static ssd1306_callback_t screen_cb_funcs;
    
int CWM_SCREEN_INIT(void)
{
    screen_cb_funcs.cbfunc_SingleRegWrite= CWM_I2CMASTER_DMA_WRITE_REG_SINGLE;
    screen_cb_funcs.cbfunc_RegWrite = CWM_I2CMASTER_DMA_WRITE_REG;
    return 0;
}

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
            TM_SSD1306_Init(&screen_cb_funcs);
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
