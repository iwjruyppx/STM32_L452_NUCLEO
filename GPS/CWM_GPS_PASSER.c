
#include "CWM_GPS_PASSER.h"

#define PARAM_TXTMAXLEN   64
#define PARAM_ARRAYSIZE   64
#define DATABUFSIZ  128

static gps_callback_t *CWMcbfuncs = NULL;

typedef struct {
    int count;
    char data[DATABUFSIZ];
}CWM_STRING_PASSER_t, *pCWM_STRING_PASSER_t;

CWM_STRING_PASSER_t StringBuff;

void CWM_GPS_PASSER_INIT(gps_callback_t *cb_funcs)
{
    CWMcbfuncs = cb_funcs;
    StringBuff.count = 0;
    memset(StringBuff.data, 0x00, DATABUFSIZ);
}
static uint8_t calcCheckSum(char *buf, size_t siz) {
    uint8_t cs = 0;
    for (int i = 0; i < siz; i++) {
        cs ^= buf[i];
    }
    return cs;
}

static int exists(char c, char *buf) {
    const size_t n = strlen(buf);
    for (int i = 0; i < n; i++) {
        if (c == buf[i]) {
            return 1;
        }
    }
    return 0;
}

/**
 * Parse a text string.
 */
static char * parse(char *src, char *des, size_t dessiz, char *delim) {
    for (int i = 0; i < dessiz; i++, src++, des++) {
        if ((*src == '\0') || (*src == '\r') || (*src == '\n')) {
            *des = '\0';
            return NULL;
        }
        if (exists(*src, delim)) {
            *des = '\0';
            return ++src;
        }
        *des = *src;
    }
    des = '\0';
    return NULL;
}

static int parseAndCallbackGGA(char *src, gps_callback_t *cbfuncs) {
    char plist[PARAM_ARRAYSIZE][PARAM_TXTMAXLEN];
    char *p = src;
    int cnt = 0;
    
    if (cbfuncs->cbfunc_gga == NULL) {
        return -1;
    }
    
    while ((p = parse(p, plist[cnt], PARAM_TXTMAXLEN, ",*")) != NULL) {
        cnt++;
    }

    if (cnt == 15) {
        gps_gga_t data;
        data.hour = (plist[1][0] - '0') * 10 + (plist[1][1] - '0') * 1;
        data.min = (plist[1][2] - '0') * 10 + (plist[1][3] - '0') * 1;
        data.sec = (plist[1][4] - '0') * 10 + (plist[1][5] - '0') * 1;

        data.latitude = atof(plist[2]);
        
        if (sscanf(plist[3], "%c", &data.ns) != 1) {
            return -3;
        }
        const char ns = data.ns;
        if ((ns != 'N') && (ns != 'S')) {
            return -4;
        }
        
        data.longitude = atof(plist[4]);
        
        if (sscanf(plist[5], "%c", &data.ew) != 1) {
            return -6;
        }
        const char ew = data.ew;
        if ((ew != 'E') && (ew != 'W')) {
            return -7;
        }

        data.position_fix = atoi(plist[6]);
        data.satellites_used = atoi(plist[7]);

        data.hdop = atof(plist[8]);
        
        data.altitude = atoi(plist[9]);
        if (strcmp(plist[10], "M") != 0) {
            return -9;
        }

        data.altitude = atoi(plist[11]);
        if (sscanf(plist[12], "%c", &data.altitude_unit) != 1) {
            return -10;
        }

        cbfuncs->cbfunc_gga(&data);
        return 0;
    }
    return -11;
}

static int parseAndCallbackGSA(char *src, gps_callback_t *cbfuncs) {
    char plist[PARAM_ARRAYSIZE][PARAM_TXTMAXLEN];
    char *p = src;
    int cnt = 0;
    
    if (cbfuncs->cbfunc_gsa == NULL) {
        return -1;
    }
    while ((p = parse(p, plist[cnt], PARAM_TXTMAXLEN, ",*")) != NULL) {
        cnt++;
    }

    if (cnt == 18) {
        gps_gsa_t data;
        data.selmode = plist[1][0];
        if ((data.selmode != 'A') && (data.selmode != 'M')) {
            return -2;
        }

        data.fix = atoi(plist[2]);
        if ((data.fix != 1) && (data.fix != 2) && (data.fix != 3)) {
            return -3;
        }

        cbfuncs->cbfunc_gsa(&data);
        return 0;
    }
    return -4;
}

static int parseAndCallbackRMC(char *src, gps_callback_t *cbfuncs) {
    char plist[PARAM_ARRAYSIZE][PARAM_TXTMAXLEN];
    char *p = src;
    int cnt = 0;
    
    if (cbfuncs->cbfunc_rmc == NULL) {
        return -1;
    }
    
    while ((p = parse(p, plist[cnt], PARAM_TXTMAXLEN, ",*")) != NULL) {
        cnt++;
    }

    if (cnt == 13) {
        gps_rmc_t data;
        data.hour = (plist[1][0] - '0') * 10 + (plist[1][1] - '0') * 1;
        data.min = (plist[1][2] - '0') * 10 + (plist[1][3] - '0') * 1;
        data.sec = (plist[1][4] - '0') * 10 + (plist[1][5] - '0') * 1;

        data.status = plist[2][0];
        
        data.nl = atof(plist[3]);
        
        if (strcmp(plist[4], "N") != 0) {
            return -3;
        }

        data.el = atof(plist[5]);
        if (strcmp(plist[6], "E") != 0) {
            return -5;
        }

        cbfuncs->cbfunc_rmc(&data);
        return 0;
    }
    return -4;
}

static int parseAndCallbackGSV(char *src, gps_callback_t *cbfuncs) {

    char plist[PARAM_ARRAYSIZE][PARAM_TXTMAXLEN];
    char *p = src;
    int cnt = 0;
    
    if (cbfuncs->cbfunc_gsv == NULL) {
        return -1;
    }
    while ((p = parse(p, plist[cnt], PARAM_TXTMAXLEN, ",*")) != NULL) {
        cnt++;
    }

    if (cnt == 20) {
        gps_gsv_t data;
        data.msgcnt = atoi(plist[1]);

        data.msgnum = atoi(plist[2]);

        data.satcnt = atoi(plist[3]);

        static const int SATINFOFS = 4;
        for (int i = 0; i < 4; i++) {
            data.satellite[i].num = atoi(plist[SATINFOFS + 0 * (i * 4)]);
            data.satellite[i].elevation = atoi(plist[SATINFOFS + 1 * (i * 4)]);
            data.satellite[i].azimuth = atoi(plist[SATINFOFS + 2 * (i * 4)]);
            data.satellite[i].snr = atoi(plist[SATINFOFS + 3 *(i * 4)]);
        }

        int chksum;
        if (sscanf(plist[20], "%x", &chksum) != 1) {
            return -2;
        }

        cbfuncs->cbfunc_gsv(&data);
        return 0;
    }
    return -3;
}

static int parseAndCallbackUnknown(char *src, gps_callback_t *cbfuncs) {
    return 0;
}

static int CMW_GPS_STRING_PASSER(char *data)
{
    int status;
    int c;
    
    if (CWMcbfuncs->cbfunc_GetNMEAString == NULL) {
        return -1;
    }
    
    /*
     * Read from a serial buffer.
     */
    do {
        status = 0;
        do {
            c = CWMcbfuncs->cbfunc_GetNMEAString();
            if (c < 0) {
                return -1;
            }
            if ((c == '\r') || (c == '\n')) {
                status = 1;
                c = '\0';
            }
            if((c == '$')){
                c = '\0';
                StringBuff.data[StringBuff.count++] = c & 0xff;
                memcpy(data, StringBuff.data, StringBuff.count);
                
                StringBuff.count = 0;
                c = '$';
                StringBuff.data[StringBuff.count++] = c & 0xff;
                return 0;
            }
            StringBuff.data[StringBuff.count++] = c & 0xff;
        } while (!status);
    } while (StringBuff.count <= 1);

    /*data update*/
    memcpy(data, StringBuff.data, StringBuff.count);
    StringBuff.count = 0;
    return 0;
}

static int CWM_GPS_CHECKSUM_VERIFY(char *src)
{

    /*
     * Check a check sum for the data. The data format is '$[DATA]*CS'.
     */
    const size_t srclen = strlen(src);
    if ((src[0] == '$') && (src[srclen - 3] == '*')) {
        int cs_src;
        if (sscanf(src + srclen - 2, "%X", &cs_src) != 1) {
            printf("Invalid check sum data found.(%s)\n", src);
            return -1;
        }
        uint8_t cs_cal = calcCheckSum(src + 1, srclen - 4);
        if ((uint8_t)cs_src != cs_cal) {
            printf("Illegal data found.(%s)\n", src);
            return -1;
        }
    } else if ((src[0] == '\0')){
        return -1;
    } else {
        printf("Invalid data format found.(%s)\n", src);
        return -1;
    }
    return 0;
}

int GPGGA;

static int CMW_GPS_PASSER(char *src)
{
    char *p = src;
    char des[DATABUFSIZ];
    int res = 0;
    
    if ((p = parse(p, des, sizeof(des), ",*")) != NULL) {
        if (strcmp(des, "$GPGGA") == 0) {
            res = parseAndCallbackGGA(src, CWMcbfuncs);
            GPGGA = res;
        } else if (strcmp(des, "$GPGSA") == 0) {
            res = parseAndCallbackGSA(src, CWMcbfuncs);
        } else if (strcmp(des, "$GPRMC") == 0) {
            res = parseAndCallbackRMC(src, CWMcbfuncs);
        } else if (strcmp(des, "$GPGSV") == 0) {
            res = parseAndCallbackGSV(src, CWMcbfuncs);
        } else {
            res = parseAndCallbackUnknown(src, CWMcbfuncs);
        }
    }

    return res;
}

int CWM_GPS_DATA_PASSER_PROCESS(void)
{
    char src[DATABUFSIZ];
    
    /*
     * Return if the callback function does not exists.
     */
    if (CWMcbfuncs == NULL) {
        return -1;
    }
    
    while(CMW_GPS_STRING_PASSER(src) == 0){
        
        /*
         * Call a function for logging data.
         */
        if (CWMcbfuncs->cbfunc_log != NULL) {
            CWMcbfuncs->cbfunc_log(src);
        }
        
        if(CWM_GPS_CHECKSUM_VERIFY(src) == 0){
            CMW_GPS_PASSER(src);
        }
    }
    return 0;
}

