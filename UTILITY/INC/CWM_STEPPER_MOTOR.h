
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
    PWM_STATES_STOP,

    /*Stepper motor mode*/
    PWM_MODE_NORMAL,
    PWM_MODE_HIGH_TORQUE,
    
}CWM_PWM_STATES_e;

typedef struct {    
    CWM_PWM_STATES_e mode;
    double stepDegrees;
    int minDelay;
    int maxDelay;
} StepperMotorInfo_t, *pStepperMotorInfo_t;

typedef struct {    
    CWM_PWM_STATES_e state;
    int maxSpeed;
    union{
        struct{
            int timeMs;
        };
        struct{
            double Degrees;
        };
    };
} StepperMotorCtrl_t, *pStepperMotorCtrl_t;

typedef struct StepperMotorH_t{
    StepperMotorInfo_t info;
    StepperMotorCtrl_t ctrlInfo;
    int stateMachine;
    int currentDelay;
    double degrees;
    void (*GPIO_1)(CWM_PWM_STATES_e);
    void (*GPIO_2)(CWM_PWM_STATES_e);
    void (*GPIO_3)(CWM_PWM_STATES_e);
    void (*GPIO_4)(CWM_PWM_STATES_e);
    void (*DELAY)(int);
    int (*Control)(struct StepperMotorH_t *, pStepperMotorCtrl_t);
    int (*Timer)(struct StepperMotorH_t *);
    int (*FinishCallBack)(struct StepperMotorH_t *);
} StepperMotorHandle_t, *pStepperMotorHandle_t;

void CWM_STEPPER_MOTOR_INIT(pStepperMotorHandle_t handle, pStepperMotorInfo_t info);

#endif /* __CWM_STEPPER_MOTOR_H */
