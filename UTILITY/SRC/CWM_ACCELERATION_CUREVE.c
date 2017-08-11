
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "CWM_ACCELERATION_CUREVE.h"

static double GetPercent(double min, double max, double input)
{
    double result = 0;
    result = ((100.0f / (max-min))*(input-min));
    if(result > 100.0f)
        result = 100.0f;
    if(result < 0.0f)
        result = 0.0f;
    return result;
}

static double PercentCV(double min, double max, double input)
{
    double result = 0;
    if(input > 100.0f)
        input = 100.0f;
    if(input >0.0f){
        result = (input/(100.0f / (max-min)))+min;
    }else{
        result = min;
    }
    return result;
}

double SM_AccCurve(double currentDegree, pAccDecelCurveInfo_t curveInfo)
{
    int SpPercent = 0;
    int i;
    SpPercent = (int)GetPercent(curveInfo->startCtDegree, curveInfo->endCtDegree, currentDegree);
    if(curveInfo->positiveNegative == 0)
        SpPercent = 100 - SpPercent;

    for(i=0;i<MAXCURVE_CUT-1;i++)
    {
        if(SpPercent > curveInfo->percent[i] && SpPercent <= curveInfo->percent[i+1])
            return (PercentCV(PercentCV(curveInfo->min, curveInfo->max, curveInfo->range[i]), PercentCV(curveInfo->min, curveInfo->max, curveInfo->range[i+1]), GetPercent(curveInfo->percent[i],curveInfo->percent[i+1],SpPercent)));

    }
    return CWM_ERROR;
}