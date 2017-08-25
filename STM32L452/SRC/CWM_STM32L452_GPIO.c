
/*Standard Utility include file*/
#include "CWM_UTILITY.h"

/*Standard Hardware include file*/
#include "CWM_PERIPHERAL_L452.h"

#include "CWM_STM32L452_GPIO.h"

void CWM_BUTTON_INIT(void)
{
    /* Configure User push-button */
    BSP_PB_Init(BUTTON_USER,BUTTON_MODE_EXTI);
}

void EXTI15_10_IRQHandler(void);
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    CWM_CMD_t data;
    
    /*Send data to host*/
    if(GPIO_Pin == USER_BUTTON_PIN)
    {
        if(HAL_GPIO_ReadPin(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN) == GPIO_PIN_SET)
            data.cmd = CWM_CMD_GPIO_RISING;
        else
            data.cmd = CWM_CMD_GPIO_FALLING;
        
        data.type = GPIO_Pin;
        CWM_MSG_QUEUE_SEND(&data);
    }
}