
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "CWM_STEPPER_MOTOR.h"

#define CONTROL_DEGREE  50.0f

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
    handle->degrees += handle->info.stepDegrees;
}

static void SM_STATE_MACHINE_HIGH_TORQUE(struct StepperMotorH_t * handle)
{
    switch(handle->stateMachine++)
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
    if(handle->stateMachine > 3)
        handle->stateMachine = 0;
    /*1 step = 0.17578125 degrees*/
    handle->degrees += handle->info.stepDegrees;
}

static int SM_Speed(struct StepperMotorH_t * handle)
{
    int cut = 0;    
    cut = (int)(CONTROL_DEGREE /(handle->info.maxDelay-handle->info.minDelay));
    if(handle->degrees < CONTROL_DEGREE)
    {
        handle->currentDelay = handle->info.maxDelay - (int)(handle->degrees/(double)cut);
    }
    else if((handle->ctrlInfo.Degrees - handle->degrees) < CONTROL_DEGREE)
    {
        handle->currentDelay = handle->info.maxDelay - (int)((handle->ctrlInfo.Degrees - handle->degrees)/(double)cut);
    }

    if(handle->currentDelay< handle->info.minDelay)
        handle->currentDelay = handle->info.minDelay;

    return 0;
}

static int SM_MotionControl(struct StepperMotorH_t * handle)
{
    if(handle->ctrlInfo.state == PWM_STATES_START_DEGREES)
    {
        if(handle->degrees >= handle->ctrlInfo.Degrees)
        {
            handle->ctrlInfo.state = PWM_STATES_STOP;
            if(NULL != handle->FinishCallBack)
                handle->FinishCallBack(handle);
            
            return CWM_SUCCESS;
        }
    }
    SM_Speed(handle);
    handle->DELAY(handle->currentDelay);
    return CWM_NON;
}

static int STEPPER_MOTOR_TIMER(struct StepperMotorH_t * handle)
{
    int rty;
    if(handle->ctrlInfo.state == PWM_STATES_STOP)
        return CWM_NON;

    rty = SM_MotionControl(handle);
    if(rty)
        return rty;
    
    if(handle->info.mode == PWM_MODE_NORMAL)
        SM_STATE_MACHINE_NORMAL(handle);
    
    if(handle->info.mode == PWM_MODE_HIGH_TORQUE)
        SM_STATE_MACHINE_HIGH_TORQUE(handle);


    return CWM_NON;
}

static int STEPPER_MOTOR_CONTROL(struct StepperMotorH_t * handle, pStepperMotorCtrl_t cmd)
{
    memcpy(&handle->ctrlInfo, cmd, sizeof(StepperMotorCtrl_t));
    handle->degrees = 0.0f;
    handle->currentDelay = handle->info.maxDelay;
    
    return CWM_NON;
}

void CWM_STEPPER_MOTOR_INIT(pStepperMotorHandle_t handle, pStepperMotorInfo_t info)
{
    handle->Control = STEPPER_MOTOR_CONTROL;
    handle->Timer= STEPPER_MOTOR_TIMER;
    handle->ctrlInfo.state = PWM_STATES_STOP;
    handle->stateMachine= 0;
    
    memcpy(&handle->info, info, sizeof(StepperMotorInfo_t));
}
