

/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_SCREEN.h"

#include "CWM_MSG_QUEUE.h"

#ifdef USE_OLED_DRIVER_SSD1306

static ssd1306_callback_t screen_cb_funcs;

static void evtcb_CWM_CMD_SCREEN_INIT(void *handle, void *evtData)
{
    TM_SSD1306_Init(&screen_cb_funcs);
}

static void evtcb_CWM_CMD_SCREEN_ON(void *handle, void *evtData)
{
    SSD1306_ON();
}

static void evtcb_CWM_CMD_SCREEN_OFF(void *handle, void *evtData)
{
    SSD1306_OFF();
}

static void evtcb_CWM_CMD_SCREEN_CLEAN(void *handle, void *evtData)
{
    TM_SSD1306_ScreenClean();
}

static void evtcb_CWM_CMD_SCREEN_UPDATE(void *handle, void *evtData)
{
    TM_SSD1306_UpdateScreen();
}

static void evtcb_CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE(void *handle, void *evtData)
{
    pCWM_CMD_t pdata = (pCWM_CMD_t)evtData;
    CWM_SSD1306_Puts_Auto_newLine(pdata->string, &TM_Font_7x10, SSD1306_COLOR_WHITE);
}

#endif /*USE_OLED_DRIVER_SSD1306*/

#ifdef USE_OLED_DRIVER_SH1106

static SH1106_callback_t screen_cb_funcs;

static void evtcb_CWM_CMD_SCREEN_INIT(void *handle, void *evtData)
{
    TM_SH1106_Init(&screen_cb_funcs);
}

static void evtcb_CWM_CMD_SCREEN_ON(void *handle, void *evtData)
{
    SH1106_ON();
}

static void evtcb_CWM_CMD_SCREEN_OFF(void *handle, void *evtData)
{
    SH1106_OFF();
}

static void evtcb_CWM_CMD_SCREEN_CLEAN(void *handle, void *evtData)
{
    TM_SH1106_ScreenClean();
}

static void evtcb_CWM_CMD_SCREEN_UPDATE(void *handle, void *evtData)
{
    TM_SH1106_UpdateScreen();
}

static void evtcb_CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE(void *handle, void *evtData)
{
    pCWM_CMD_t pdata = (pCWM_CMD_t)evtData;
    CWM_SH1106_Puts_Auto_newLine(pdata->string, &TM_Font_7x10, SH1106_COLOR_WHITE);
}

#endif /*USE_OLED_DRIVER_SH1106*/

int CWM_SCREEN_INIT(void)
{
#ifdef USE_OLED_DRIVER_SSD1306
    /*SetUp peripheral interface*/
    screen_cb_funcs.cbfunc_SingleRegWrite= CWM_I2CMASTER_DMA_WRITE_REG_SINGLE;
    screen_cb_funcs.cbfunc_RegWrite = CWM_I2CMASTER_DMA_WRITE_REG;

    /*Event listen register*/
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_SCREEN_INIT, NULL, evtcb_CWM_CMD_SCREEN_INIT);
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_SCREEN_ON, NULL, evtcb_CWM_CMD_SCREEN_ON);
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_SCREEN_OFF, NULL, evtcb_CWM_CMD_SCREEN_OFF);
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_SCREEN_CLEAN, NULL, evtcb_CWM_CMD_SCREEN_CLEAN);
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_SCREEN_UPDATE, NULL, evtcb_CWM_CMD_SCREEN_UPDATE);
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE, NULL, evtcb_CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE);
#endif /*USE_OLED_DRIVER_SSD1306*/

#ifdef USE_OLED_DRIVER_SH1106
    /*SetUp peripheral interface*/
    pI2cClass_t i2cHandle = CWM_GET_I2C_HANDLE(CWM_I2C1);
    if(NULL != i2cHandle)
    {
        screen_cb_funcs.cbfunc_SingleRegWrite= i2cHandle->singleRegWrite;
        screen_cb_funcs.cbfunc_RegWrite = i2cHandle->Write;
    }

    /*Event listen register*/
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_SCREEN_INIT, NULL, evtcb_CWM_CMD_SCREEN_INIT);
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_SCREEN_ON, NULL, evtcb_CWM_CMD_SCREEN_ON);
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_SCREEN_OFF, NULL, evtcb_CWM_CMD_SCREEN_OFF);
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_SCREEN_CLEAN, NULL, evtcb_CWM_CMD_SCREEN_CLEAN);
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_SCREEN_UPDATE, NULL, evtcb_CWM_CMD_SCREEN_UPDATE);
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE, NULL, evtcb_CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE);
#endif /*USE_OLED_DRIVER_SH1106*/

    return 0;
}
