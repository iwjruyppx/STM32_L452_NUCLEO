
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

#include "CWM_STEPPER_MOTOR.h"

#define STEPPER_MOTOR_MODE 1

#if ( STEPPER_MOTOR_MODE == 1 ) 
static void STEPPER_MOTOR_LLHH(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_LOW);
    handle->GPIO_2(PWM_STATES_LOW);
    handle->GPIO_3(PWM_STATES_HIGH);
    handle->GPIO_4(PWM_STATES_HIGH);
}
static void STEPPER_MOTOR_HLLH(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_HIGH);
    handle->GPIO_2(PWM_STATES_LOW);
    handle->GPIO_3(PWM_STATES_LOW);
    handle->GPIO_4(PWM_STATES_HIGH);
}

static void STEPPER_MOTOR_HHLL(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_HIGH);
    handle->GPIO_2(PWM_STATES_HIGH);
    handle->GPIO_3(PWM_STATES_LOW);
    handle->GPIO_4(PWM_STATES_LOW);
}

static void STEPPER_MOTOR_LHHL(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_LOW);
    handle->GPIO_2(PWM_STATES_HIGH);
    handle->GPIO_3(PWM_STATES_HIGH);
    handle->GPIO_4(PWM_STATES_LOW);
}
#endif

#if ( STEPPER_MOTOR_MODE == 2 ) 
static void STEPPER_MOTOR_LHHH(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_LOW);
    handle->GPIO_2(PWM_STATES_LOW);
    handle->GPIO_3(PWM_STATES_HIGH);
    handle->GPIO_4(PWM_STATES_HIGH);
}
static void STEPPER_MOTOR_HLHH(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_HIGH);
    handle->GPIO_2(PWM_STATES_LOW);
    handle->GPIO_3(PWM_STATES_LOW);
    handle->GPIO_4(PWM_STATES_HIGH);
}

static void STEPPER_MOTOR_HHLH(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_HIGH);
    handle->GPIO_2(PWM_STATES_HIGH);
    handle->GPIO_3(PWM_STATES_LOW);
    handle->GPIO_4(PWM_STATES_LOW);
}

static void STEPPER_MOTOR_HHHL(struct StepperMotorH_t * handle)
{
    handle->GPIO_1(PWM_STATES_LOW);
    handle->GPIO_2(PWM_STATES_HIGH);
    handle->GPIO_3(PWM_STATES_HIGH);
    handle->GPIO_4(PWM_STATES_LOW);
}
#endif

static int STEPPER_MOTOR_TIMER(struct StepperMotorH_t * handle)
{
    if(handle->currentState == PWM_STATES_STOP)
        return CWM_NON;

    switch(handle->stateMachine++)
    {
    
#if ( STEPPER_MOTOR_MODE == 1 ) 
        case 0:
            STEPPER_MOTOR_LLHH(handle);
            break;
        case 1:
            STEPPER_MOTOR_HLLH(handle);
            break;
        case 2:
            STEPPER_MOTOR_HHLL(handle);
            break;
        case 3:
            STEPPER_MOTOR_LHHL(handle);
            break;
        default:
            handle->stateMachine = 0;
            break;
#endif
#if ( STEPPER_MOTOR_MODE == 2 ) 
        case 0:
            STEPPER_MOTOR_LHHH(handle);
            break;
        case 1:
            STEPPER_MOTOR_HLHH(handle);
            break;
        case 2:
            STEPPER_MOTOR_HHLH(handle);
            break;
        case 3:
            STEPPER_MOTOR_HHHL(handle);
            break;
        default:
            handle->stateMachine = 0;
            break;
#endif
            
    }
    if(handle->stateMachine > 3)
        handle->stateMachine = 0;
    return CWM_NON;
}

static int STEPPER_MOTOR_CONTROL(struct StepperMotorH_t * handle, CWM_PWM_STATES_e status, int timeMs)
{
    handle->currentState = status;
    return CWM_NON;
}

void CWM_STEPPER_MOTOR_INIT(pStepperMotorHandle_t handle)
{
    handle->Control = STEPPER_MOTOR_CONTROL;
    handle->Timer= STEPPER_MOTOR_TIMER;
    handle->currentState = PWM_STATES_STOP;
    handle->stateMachine= 0;
}
