
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_TASK2.h"

#define Task_Name "CWMTask2"

#include "CWM_GPS.h"

extern int64_t gTimestamp;

StepperMotorHandle_t STEP_HANDLE;

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

static void STEPPER_MOTOR_INIT(void)
{
    
    GPIO_InitTypeDef  GPIO_InitStruct;

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

    STEP_HANDLE.GPIO_1 = STEP_GPIO1;
    STEP_HANDLE.GPIO_2 = STEP_GPIO2;
    STEP_HANDLE.GPIO_3 = STEP_GPIO3;
    STEP_HANDLE.GPIO_4 = STEP_GPIO4;
    CWM_STEPPER_MOTOR_INIT(&STEP_HANDLE);

}

static void Task2Start(void)
{
    /*Enable Usart3 RX listen*/
    CWM_INTERNAL_CMD_SET_CT(CWM_CMD_USART_LISTEN, CWM_USART3);
    /*Enable Uart4 RX listen*/
    CWM_INTERNAL_CMD_SET_CT(CWM_CMD_USART_LISTEN, CWM_UART4);
    
    /*OLED Driver Init*/
    CWM_INTERNAL_CMD_SET(CWM_CMD_SCREEN_INIT);
    CWM_INTERNAL_CMD_SET(CWM_CMD_SCREEN_ON);

    STEPPER_MOTOR_INIT();
    STEP_HANDLE.Control(&STEP_HANDLE, PWM_STATES_START, 0);
}

static void Task2(const void *argument)
{  
    int loop_count = 0;
    Task2Start();
    osDelay(1000);
    for (;;)
    {
        if(loop_count > 2047)
        {
            osDelay(1000);
            loop_count = 0;
#if 1
            showGpsInfo();
#endif
        }
        STEP_HANDLE.Timer(&STEP_HANDLE);
        loop_count++;
        osDelay(2);
    }
}

void CWM_TASK2_INIT(void)
{
    osThreadDef(Task_Name, Task2, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(Task_Name), NULL);
}

