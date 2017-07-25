
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
    PWM_STATES_LOW = 0,
    PWM_STATES_HIGH,
    PWM_STATES_START,
    PWM_STATES_STOP,
}CWM_PWM_STATES_e;

typedef struct StepperMotorH_t{
    void (*GPIO_1)(CWM_PWM_STATES_e);
    void (*GPIO_2)(CWM_PWM_STATES_e);
    void (*GPIO_3)(CWM_PWM_STATES_e);
    void (*GPIO_4)(CWM_PWM_STATES_e);
    
    int (*Control)(struct StepperMotorH_t *, CWM_PWM_STATES_e, int);
    int (*Timer)(struct StepperMotorH_t *);
    CWM_PWM_STATES_e currentState;
    int stateMachine;
} StepperMotorHandle_t, *pStepperMotorHandle_t;

void CWM_STEPPER_MOTOR_INIT(pStepperMotorHandle_t handle);

#endif /* __CWM_STEPPER_MOTOR_H */
