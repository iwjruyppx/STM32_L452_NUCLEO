
#ifndef __CWM_GPS_H
#define __CWM_GPS_H

#include "CWM_GPS_PASSER.h"

typedef struct {
    gps_gga_t   gga;
    gps_gsa_t   gsa;
    gps_gsv_t   gsv;
    gps_rmc_t   rmc;
}CWM_GPS_INFO, *pCWM_GPS_INFO;

void CWM_GPS_INIT(void);

#endif /* __CWM_GPS_H */
