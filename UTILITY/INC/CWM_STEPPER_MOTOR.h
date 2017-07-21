
#ifndef __CWM_STEPPER_MOTOR_H
#define __CWM_STEPPER_MOTOR_H

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
