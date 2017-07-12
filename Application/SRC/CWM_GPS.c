
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_GPS.h"

#include "CWM_UART_QUEUE.h"

#ifdef USE_GPS
static gps_callback_t gps_cb_funcs;

static CWM_GPS_INFO GPS_INFO;

static int LOGE(const char * format,...){
    va_list argList;
    CWM_CMD_t data;

    va_start(argList,format);
    vsnprintf(data.string,sizeof(data.string),format,argList);
    va_end(argList);
    data.cmd = CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE;
    CWM_INTERNAL_CMD_SET_T(&data);
    return 0;
}

static void GPS_LOG_LISTEN(char *str)
{
}
static void GPS_GGA_LISTEN(gps_gga_t *p)
{
    memcpy(&GPS_INFO.gga, p, sizeof(gps_gga_t));
}
static void GPS_GSA_LISTEN(gps_gsa_t *p)
{
    memcpy(&GPS_INFO.gsa, p, sizeof(gps_gsa_t));
}
static void GPS_GSV_LISTEN(gps_gsv_t *p)
{
    memcpy(&GPS_INFO.gsv, p, sizeof(gps_gsv_t));
}
static void GPS_RMC_LISTEN(gps_rmc_t *p)
{
    memcpy(&GPS_INFO.rmc, p, sizeof(gps_rmc_t));
}

void showGpsInfo(void)
{
    CWM_INTERNAL_CMD_SET(CWM_CMD_SCREEN_CLEAN);
    #if 1
    LOGE("T:%d:%d:%d,%X\n",GPS_INFO.rmc.hour,GPS_INFO.rmc.min,GPS_INFO.rmc.sec,GPS_INFO.rmc.status);
    LOGE("NL:%s\n",doubleToString(GPS_INFO.rmc.nl));
    LOGE("EL:%s\n",doubleToString(GPS_INFO.rmc.el));
    #endif
    CWM_INTERNAL_CMD_SET(CWM_CMD_SCREEN_UPDATE);
}

static void evtcb_CWM_CMD_UART_LISTEN_RX_UPDATE(void *handle, void *evtData)
{
    CWM_GPS_DATA_PASSER_PROCESS();
}
void CWM_GPS_INIT(void)
{
    /*GPS event listen register*/
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_UART4_RX_UPDATE, NULL, evtcb_CWM_CMD_UART_LISTEN_RX_UPDATE);

    /*GPS passer register*/
    gps_cb_funcs.cbfunc_GetNMEAString = GET_GPS_UART_STRING;
    gps_cb_funcs.cbfunc_log = GPS_LOG_LISTEN;
    gps_cb_funcs.cbfunc_gga = GPS_GGA_LISTEN;
    gps_cb_funcs.cbfunc_gsa = GPS_GSA_LISTEN;
    gps_cb_funcs.cbfunc_gsv = GPS_GSV_LISTEN;
    gps_cb_funcs.cbfunc_rmc = GPS_RMC_LISTEN;
    
    CWM_GPS_PASSER_INIT(&gps_cb_funcs);
}
#else
void CWM_GPS_INIT(void)
{
}
#endif/*USE_GPS*/
