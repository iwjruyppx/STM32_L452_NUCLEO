
#ifndef __CWM_GPS_PASSER_H
#define __CWM_GPS_PASSER_H

#include "CWM_UTILITY.h"

/**
 * GGA - Global Positioning System Fixed Data.
 *
 * $GPGGA,161229.487,3723.2475,N,12158.3416,W,1,07,1.0,9.0,M, , , ,0000*18
 */
typedef struct {
    int hour;
    int min;
    int sec;
    double latitude;
    char ns;
    double longitude;
    char ew;
    int position_fix;
    int satellites_used;
    double hdop;
    int altitude;
    char altitude_unit;
} gps_gga_t;

/**
 * GSA&#226;&#8364;&#129;hGNSS DOP and Active Satellites.
 *
 * $GPGSA,A,3,07,02,26,27,09,04,15, , , , , ,1.8,1.0,1.5*33
 */
typedef struct {
    char selmode;
    int fix;
} gps_gsa_t;

/**
 * for RMC:
 *  Time, date, position, course and speed data.
 */
typedef struct {
    int hour;
    int min;
    int sec;
    char status;
    double nl;
    double el;
} gps_rmc_t;

/**
 * for GSV:
 *  The number of GPS satellites in view satellite ID numbers,
 *  elevation, azimuth, and SNR values.
 */
typedef struct {
    int num;
    int elevation;
    int azimuth;
    int snr;
} gps_gsv_satellite_t;

/**
 * for GSV:
 *  The number of GPS satellites in view satellite ID numbers,
 *  elevation, azimuth, and SNR values.
 */
typedef struct {
    int msgcnt;
    int msgnum;
    int satcnt;
    gps_gsv_satellite_t satellite[4];
} gps_gsv_t;

typedef struct {
    /**
     * A callback function for logging data.
     */
    void (*cbfunc_log)(char *str);

    /**
     * A callback function for GGA.
     *
     * GGA - Global Positioning System Fixed Data.
     */
    void (*cbfunc_gga)(gps_gga_t *p);

    /**
     * A callback function for GLL.
     *
     * GLL - Geographic Position - Latitude/Longitude.
     */
    // TODO

    /**
     * A callback function for GSA.
     *
     * GSA - GNSS DOP and Active Satellites.
     */
    void (*cbfunc_gsa)(gps_gsa_t *p);

    /**
     * A callback function for GSV.
     *
     * GSV - GNSS Satellites in View.
     */
    void (*cbfunc_gsv)(gps_gsv_t *p);

    /**
     * A callback function for MSS.
     *
     * MSS - MSK Receiver Signal.
     */
    // TODO

    /**
     * A callback function for RMC.
     *
     * RMC - Recommended Minimum Specific GNSS Data.
     */
    void (*cbfunc_rmc)(gps_rmc_t *p);

    /**
     * A callback function for VTG.
     *
     * VTG - Course Over Ground and Ground Speed.
     */
    // TODO

    /**
     * A callback function for ZDA.
     *
     * ZDA - SiRF Timing Message.
     */
    // TODO

} gps_callback_t;

void CWM_GPS_PASSER_INIT(gps_callback_t *cb_funcs);

int CWM_GPS_DATA_PASSER_PROCESS(void);

#endif /* __CWM_GPS_PASSER_H */
