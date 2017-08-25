
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "CWM_STEPPER_MOTOR.h"

#include "CWM_ACCELERATION_CUREVE.h"

#define SPMINDELAY handle->spInfo.minDelay
#define SPMAXDELAY handle->spInfo.maxDelay
#define SPCURDELAY handle->currentDelay
#define SPCURDEG handle->degrees

static void SM_LLHH(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_LOW);
    handle->GPIO_2(PWM_STATES_LOW);
    handle->GPIO_3(PWM_STATES_HIGH);
    handle->GPIO_4(PWM_STATES_HIGH);
}
static void SM_HLLH(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_HIGH);
    handle->GPIO_2(PWM_STATES_LOW);
    handle->GPIO_3(PWM_STATES_LOW);
    handle->GPIO_4(PWM_STATES_HIGH);
}

static void SM_HHLL(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_HIGH);
    handle->GPIO_2(PWM_STATES_HIGH);
    handle->GPIO_3(PWM_STATES_LOW);
    handle->GPIO_4(PWM_STATES_LOW);
}

static void SM_LHHL(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_LOW);
    handle->GPIO_2(PWM_STATES_HIGH);
    handle->GPIO_3(PWM_STATES_HIGH);
    handle->GPIO_4(PWM_STATES_LOW);
}

static void SM_LHHH(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_LOW);
    handle->GPIO_2(PWM_STATES_LOW);
    handle->GPIO_3(PWM_STATES_HIGH);
    handle->GPIO_4(PWM_STATES_HIGH);
}
static void SM_HLHH(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_HIGH);
    handle->GPIO_2(PWM_STATES_LOW);
    handle->GPIO_3(PWM_STATES_LOW);
    handle->GPIO_4(PWM_STATES_HIGH);
}

static void SM_HHLH(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_HIGH);
    handle->GPIO_2(PWM_STATES_HIGH);
    handle->GPIO_3(PWM_STATES_LOW);
    handle->GPIO_4(PWM_STATES_LOW);
}

static void SM_HHHL(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_LOW);
    handle->GPIO_2(PWM_STATES_HIGH);
    handle->GPIO_3(PWM_STATES_HIGH);
    handle->GPIO_4(PWM_STATES_LOW);
}

static void SM_STATE_MACHINE_NORMAL(struct StepperMotorH_t * handle)
{
    switch(handle->stateMachine++)

    {
        case 0:
            SM_LHHH(handle);
            break;
        case 1:
            SM_HLHH(handle);
            break;
        case 2:
            SM_HHLH(handle);
            break;
        case 3:
            SM_HHHL(handle);
            break;
        default:
            handle->stateMachine = 0;
            break;
    }
    if(handle->stateMachine > 3)
        handle->stateMachine = 0;
    /*1 step = 0.17578125 degrees*/
    handle->degrees += handle->spInfo.stepDegrees;
}

static void SM_STATE_MACHINE_HIGH_TORQUE(struct StepperMotorH_t * handle)
{
    switch(handle->stateMachine)
    {
        case 0:
            SM_LLHH(handle);
            break;
        case 1:
            SM_HLLH(handle);
            break;
        case 2:
            SM_HHLL(handle);
            break;
        case 3:
            SM_LHHL(handle);
            break;
        default:
            handle->stateMachine = 0;
            break;
    }
    
    if(handle->ctrlInfo.rotate == PWM_ROTATE_POSITIVE)
    {
        handle->stateMachine++;
    }else{
        handle->stateMachine--;
    }
        
    if(handle->stateMachine > 3)
        handle->stateMachine = 0;
    if(handle->stateMachine < 0)
        handle->stateMachine = 3;
    
    /*1 step = 0.17578125 degrees*/
    handle->degrees += handle->spInfo.stepDegrees;
}

#define CTRL_RANGE 360.0f + 90.0f

static int SM_Speed(struct StepperMotorH_t * handle)
{
    AccDecelCurveInfo_t startCurveInfo = {.positiveNegative = 0, .min = handle->spInfo.minDelay, .max =handle->spInfo.maxDelay, .startCtDegree = 0.0f, .endCtDegree = CTRL_RANGE, .percent = {-1, 30, 50, 70, 80, 100}, .range = {0.0f, 10.0f, 25.0f, 50.0f, 80.0f, 100.0f} };
    AccDecelCurveInfo_t endcurveInfo = {.positiveNegative = 1, .min = handle->spInfo.minDelay, .max =handle->spInfo.maxDelay, .startCtDegree = handle->ctrlInfo.Degrees -CTRL_RANGE, .endCtDegree = handle->ctrlInfo.Degrees,  .percent = {-1, 30, 60, 80, 90, 100}, .range = {0.0f, 10.0f, 25.0f, 50.0f, 80.0f, 100.0f} };
    
    if(handle->degrees < startCurveInfo.endCtDegree)
    {
        handle->currentDelay = SM_AccCurve(SPCURDEG, &startCurveInfo);
    }
    else if(endcurveInfo.startCtDegree < handle->degrees)
    {
        handle->currentDelay = SM_AccCurve(SPCURDEG, &endcurveInfo);
    }
#if 0
    if(handle->currentDelay< handle->spInfo.minDelay)
        handle->currentDelay = handle->spInfo.minDelay;
#endif

    return 0;
}

static int SM_MotionControl(struct StepperMotorH_t * handle)
{
    if(handle->ctrlInfo.state == PWM_STATES_START_DEGREES)
    {
        if(handle->degrees >= (handle->ctrlInfo.Degrees - handle->spInfo.stepDegrees))
        {
            handle->ctrlInfo.state = PWM_STATES_PRESTOP;
            if(NULL != handle->FinishCallBack)
                handle->FinishCallBack(handle);
            
            return CWM_SUCCESS;
        }
    }
    SM_Speed(handle);
    handle->DELAY((int)handle->currentDelay);
    return CWM_NON;
}

static int STEPPER_MOTOR_TIMER(struct StepperMotorH_t * handle)
{
    int rty;
    if(handle->ctrlInfo.state == PWM_STATES_PRESTOP)
    {
        if(handle->timeout >= 3000)
        {
            handle->ctrlInfo.state = PWM_STATES_STOP;
            handle->timeout = 0;
        }
        handle->timeout += handle->currentDelay;
        handle->DELAY((int)handle->currentDelay);
        return CWM_NON;
    }else if(handle->ctrlInfo.state == PWM_STATES_STOP)
    {
        return CWM_NON;
    }

    if(handle->spInfo.mode == PWM_MODE_NORMAL)
        SM_STATE_MACHINE_NORMAL(handle);
    
    if(handle->spInfo.mode == PWM_MODE_HIGH_TORQUE)
        SM_STATE_MACHINE_HIGH_TORQUE(handle);
    
    rty = SM_MotionControl(handle);
    if(rty)
        return rty;


    return CWM_NON;
}

static int STEPPER_MOTOR_CONTROL(struct StepperMotorH_t * handle, pStepperMotorCtrl_t cmd)
{
    memcpy(&handle->ctrlInfo, cmd, sizeof(StepperMotorCtrl_t));
    handle->degrees = 0.0f;
    handle->currentDelay = handle->spInfo.maxDelay;
    
    return CWM_NON;
}

void CWM_STEPPER_MOTOR_INIT(pStepperMotorHandle_t handle, pSteppMotorInfo_t info)
{
    handle->Control = STEPPER_MOTOR_CONTROL;
    handle->Timer= STEPPER_MOTOR_TIMER;
    handle->ctrlInfo.state = PWM_STATES_STOP;
    handle->stateMachine= 0;
    handle->timeout= 0;
    
    memcpy(&handle->spInfo, info, sizeof(SteppMotorInfo_t));
}
