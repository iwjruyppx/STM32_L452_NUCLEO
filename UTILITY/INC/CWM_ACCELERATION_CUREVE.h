
#ifndef __CWM_ACCELERATION_CUREVE_H
#define __CWM_ACCELERATION_CUREVE_H

#define MAXCURVE_CUT 6

typedef struct {
    int positiveNegative;
    double startCtDegree;
    double endCtDegree;
    double min;
    double max;
    int percent[MAXCURVE_CUT];
    int range[MAXCURVE_CUT];
} AccDecelCurveInfo_t, *pAccDecelCurveInfo_t;

double SM_AccCurve(double currentDegree, pAccDecelCurveInfo_t curveInfo);

#endif /* __CWM_ACCELERATION_CUREVE_H */
