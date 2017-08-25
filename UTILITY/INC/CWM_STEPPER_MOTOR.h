
#ifndef __CWM_STEPPER_MOTOR_H
#define __CWM_STEPPER_MOTOR_H

/*****************************************
Current stepper motor:
Stepping angle: 5.625 x1 / 64
stepper motor Turn around 360 degrees will really go ahead 5.625 degrees.
So if we want to turn around 360 degrees, we need run 2048 step.

1    :0.17578125
2    :0.3515625
4    :0.703125
8    :1.40625
16   :2.8125
32   :5.625
64   :11.25
128  :22.5
256  :45
512  :90
1024 :180
2048 : 360

*****************************************/

typedef enum {
    /*GPIO state*/
    PWM_STATES_LOW = 0,
    PWM_STATES_HIGH,
    
    /*Control*/
    PWM_STATES_START_STEP,
    PWM_STATES_START_DEGREES,
    PWM_STATES_PRESTOP,
    PWM_STATES_STOP,

    /*Stepper motor mode*/
    PWM_MODE_NORMAL,
    PWM_MODE_HIGH_TORQUE,
    PWM_MODE_HIGH_TORQUE_ROTATION,

    /*Rotate*/
    PWM_ROTATE_POSITIVE,
    PWM_ROTATE_NEGATIVE,
    
}CWM_PWM_STATES_e;

typedef struct {    
    CWM_PWM_STATES_e state;
    int maxSpeed;
    int rotate;
    union{
        struct{
            int timeMs;
        };
        struct{
            double Degrees;
        };
    };
} StepperMotorCtrl_t, *pStepperMotorCtrl_t;

typedef struct {    
    CWM_PWM_STATES_e mode;
    double stepDegrees;
    int minDelay;
    int maxDelay;
} SteppMotorInfo_t, *pSteppMotorInfo_t;

typedef struct StepperMotorH_t {
    void (*GPIO_1)(CWM_PWM_STATES_e);
    void (*GPIO_2)(CWM_PWM_STATES_e);
    void (*GPIO_3)(CWM_PWM_STATES_e);
    void (*GPIO_4)(CWM_PWM_STATES_e);
    void (*DELAY)(int);
    int (*Control)(struct StepperMotorH_t *, pStepperMotorCtrl_t);
    int (*Timer)(struct StepperMotorH_t *);
    int (*FinishCallBack)(struct StepperMotorH_t *);
    SteppMotorInfo_t spInfo;
    StepperMotorCtrl_t ctrlInfo;
    int stateMachine;
    double currentDelay;
    double degrees;
    int timeout;
} StepperMotorHandle_t, *pStepperMotorHandle_t;

void CWM_STEPPER_MOTOR_INIT(pStepperMotorHandle_t handle, pSteppMotorInfo_t info);

#endif /* __CWM_STEPPER_MOTOR_H */
