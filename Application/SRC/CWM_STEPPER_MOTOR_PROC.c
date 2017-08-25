
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_STEPPER_MOTOR_PROC.h"

#if 0
#define USER_TIMER3
#endif

typedef struct {
    StepperMotorHandle_t handle;
    pTimerClass_t tim;
    int64_t buttonPushTime;
}CWM_STEP_MOTOR_CONTROL_t;
static CWM_STEP_MOTOR_CONTROL_t CWM_SM;

#define STEP_GPIO1_TYPE GPIOB
#define STEP_GPIO1_PIN GPIO_PIN_1

#define STEP_GPIO2_TYPE GPIOB
#define STEP_GPIO2_PIN GPIO_PIN_15

#define STEP_GPIO3_TYPE GPIOB
#define STEP_GPIO3_PIN GPIO_PIN_14

#define STEP_GPIO4_TYPE GPIOB
#define STEP_GPIO4_PIN GPIO_PIN_13

static void STEP_GPIO1(CWM_PWM_STATES_e state)
{
    if(PWM_STATES_HIGH == state)
        HAL_GPIO_WritePin(STEP_GPIO1_TYPE, STEP_GPIO1_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(STEP_GPIO1_TYPE, STEP_GPIO1_PIN, GPIO_PIN_RESET);
}

static void STEP_GPIO2(CWM_PWM_STATES_e state)
{
    if(PWM_STATES_HIGH == state)
        HAL_GPIO_WritePin(STEP_GPIO2_TYPE, STEP_GPIO2_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(STEP_GPIO2_TYPE, STEP_GPIO2_PIN, GPIO_PIN_RESET);
}

static void STEP_GPIO3(CWM_PWM_STATES_e state)
{
    if(PWM_STATES_HIGH == state)
        HAL_GPIO_WritePin(STEP_GPIO3_TYPE, STEP_GPIO3_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(STEP_GPIO3_TYPE, STEP_GPIO3_PIN, GPIO_PIN_RESET);
}

static void STEP_GPIO4(CWM_PWM_STATES_e state)
{
    if(PWM_STATES_HIGH == state)
        HAL_GPIO_WritePin(STEP_GPIO4_TYPE, STEP_GPIO4_PIN, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(STEP_GPIO4_TYPE, STEP_GPIO4_PIN, GPIO_PIN_RESET);
}

static void STEP_DELAY(int delay)
{
#ifdef USER_TIMER3
    static uint32_t currentDelay = 0;
    if(currentDelay != delay)
    {
        currentDelay = delay;
        CWM_SM.tim->setPeriod(delay);
    }
#else
    osDelay(delay);
#endif
}

static int LOGE(const char * format,...){
    va_list argList;
    CWM_CMD_t data;

    va_start(argList,format);
    vsnprintf(data.string,sizeof(data.string),format,argList);
    va_end(argList);
    data.cmd = CWM_CMD_SCREEN_WRITE_AUTO_NEW_LINE;
    CWM_INTERNAL_CMD_SET_T(&data);
    return 0;
}

void ShowCurrneDegrees(void)
{
    CWM_INTERNAL_CMD_SET(CWM_CMD_SCREEN_CLEAN);
    #if 1
    LOGE("Degrees:\n%s\n",doubleToString(CWM_SM.handle.degrees));
    #endif
    CWM_INTERNAL_CMD_SET(CWM_CMD_SCREEN_UPDATE);
}

void STEPPER_MOTOR_PROC(void)
{
    CWM_SM.handle.Timer(&CWM_SM.handle);
}

void SetDegrees(int degrees)
{
    StepperMotorCtrl_t cmd;    
    cmd.state = PWM_STATES_START_DEGREES;
    if(degrees >= 0 )
    {
        cmd.rotate = PWM_ROTATE_POSITIVE;
    }else{
        cmd.rotate = PWM_ROTATE_NEGATIVE;
        degrees *= -1;
    }
    cmd.Degrees = degrees;
    CWM_SM.handle.Control(&CWM_SM.handle, &cmd);
}
static int STEPPER_MOTOR_CALLBACK(struct StepperMotorH_t * handle)
{
#if 0
    SetDegrees(360*10);
#endif
    return CWM_NON;
}

#ifdef USER_TIMER3
static void CWM_TIME3_IRQ_CALLBACK(void *info)
{
    CWM_SM.handle.Timer(&CWM_SM.handle);
}
#endif

static void evtcb_CWM_CMD_GPIO_RISING(void *handle, void *evtData)
{
    pCWM_CMD_t data = (pCWM_CMD_t)evtData;

    if(data->type == USER_BUTTON_PIN)
    {
        if(CWM_SM.buttonPushTime != 0)
        {
            if((gTimestamp - CWM_SM.buttonPushTime) > 1000)
                SetDegrees(-3600);
            else
                SetDegrees(3600);
        }
        CWM_SM.buttonPushTime = 0;
    }
}

static void evtcb_CWM_CMD_GPIO_FALLING(void *handle, void *evtData)
{
    pCWM_CMD_t data = (pCWM_CMD_t)evtData;

    if(data->type == USER_BUTTON_PIN)
    {
        CWM_SM.buttonPushTime = gTimestamp;
    }
}

void STEPPER_MOTOR_INIT(void)
{
    
    GPIO_InitTypeDef  GPIO_InitStruct;
    SteppMotorInfo_t info;

    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* -2- Configure IO in output push-pull mode to drive external LEDs */
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin = STEP_GPIO1_PIN;
    HAL_GPIO_Init(STEP_GPIO1_TYPE, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = STEP_GPIO2_PIN;
    HAL_GPIO_Init(STEP_GPIO2_TYPE, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = STEP_GPIO3_PIN;
    HAL_GPIO_Init(STEP_GPIO3_TYPE, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = STEP_GPIO4_PIN;
    HAL_GPIO_Init(STEP_GPIO4_TYPE, &GPIO_InitStruct);

    CWM_SM.handle.GPIO_1 = STEP_GPIO1;
    CWM_SM.handle.GPIO_2 = STEP_GPIO2;
    CWM_SM.handle.GPIO_3 = STEP_GPIO3;
    CWM_SM.handle.GPIO_4 = STEP_GPIO4;
    CWM_SM.handle.DELAY = STEP_DELAY;
    CWM_SM.handle.FinishCallBack= STEPPER_MOTOR_CALLBACK;
    
    info.mode = PWM_MODE_HIGH_TORQUE;
//    info.stepDegrees = 0.17578125f;
    info.stepDegrees = 1.8f;
#ifdef USER_TIMER3
    info.minDelay = 13;
    info.maxDelay = 20;
#else
    info.minDelay = 8;
    info.maxDelay = 50;
#endif
    CWM_STEPPER_MOTOR_INIT(&CWM_SM.handle, &info);

//    SetDegrees(3600);

#ifdef USER_TIMER3
    /*Timer3 get entry and register callback*/
    CWM_SM.tim = CWM_GET_TIMER_ENTRY(CWM_TIMER3);
    CWM_TIMER_REG_CALL_BACK(CWM_TIMER3, CWM_TIME3_IRQ_CALLBACK);
    CWM_SM.tim->deInit();
    CWM_SM.tim->init();
    
    CWM_SM.tim->setPeriod(10);
    CWM_SM.tim->stop();
    CWM_SM.tim->resetCount();
    CWM_SM.tim->start();
#endif
    CWM_SM.buttonPushTime = 0;
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_GPIO_RISING, NULL, evtcb_CWM_CMD_GPIO_RISING);
    CWM_MSG_QUEUE_REGISTERED(CWM_CMD_GPIO_FALLING, NULL, evtcb_CWM_CMD_GPIO_FALLING);
}
