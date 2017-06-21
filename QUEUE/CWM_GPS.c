
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "CWM_GPS.h"

#include "CWM_GPS_PASSER.h"
#include "CWM_UART_QUEUE.h"

static gps_callback_t gps_cb_funcs;

static CWM_GPS_INFO GPS_INFO;

static int get_buff_string(void)
{
    uint8_t data;
    if(CWM_UART_QUEUE_GET(&data, 1))
        return -1;
    return data;
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
void CWM_GPS_INIT(void)
{
    gps_cb_funcs.cbfunc_GetNMEAString = get_buff_string;
    gps_cb_funcs.cbfunc_log = GPS_LOG_LISTEN;
    gps_cb_funcs.cbfunc_gga = GPS_GGA_LISTEN;
    gps_cb_funcs.cbfunc_gsa = GPS_GSA_LISTEN;
    gps_cb_funcs.cbfunc_gsv = GPS_GSV_LISTEN;
    gps_cb_funcs.cbfunc_rmc = GPS_RMC_LISTEN;
    
    CWM_GPS_PASSER_INIT(&gps_cb_funcs);
}
