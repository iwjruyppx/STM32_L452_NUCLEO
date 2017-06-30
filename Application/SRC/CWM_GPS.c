
#include "CWM_GPS.h"
#include "CWM_STM32L452_UART.h"

#include "CWM_MSG_QUEUE.h"
#include "CWM_UART_QUEUE.h"

static gps_callback_t gps_cb_funcs;

static CWM_GPS_INFO GPS_INFO;

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
