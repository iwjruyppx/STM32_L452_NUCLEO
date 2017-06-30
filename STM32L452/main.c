

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "CWM_STM32L452_I2C.h"
#include "CWM_STM32L452_UART.h"
#include "CWM_STM32L452_TIMER.h"

#include "CWM_SCREEN.h"
#include "CWM_CMD_QUEUE.h"

#include "CWM_TASK1.h"
#include "CWM_TASK2.h"
#include "CWM_GPS.h"
#include "CWM_MSG_QUEUE.h"

int64_t gTimestamp = 0ll;
/*
    __disable_interrupt();
    __DSB();
    __ISB();
    __enable_interrupt();
    */

void SystemClock_Config(void);

static void UTILITY_INIT(void)
{
    CWM_MSG_QUEUE_INIT();
}
static void PLATFORM_INIT(void)
{

    HAL_Init();

    /* Configure the System clock to 80 MHz */
    SystemClock_Config();

    /* Configure LED2 */
    BSP_LED_Init(LED2);

    CWM_TIMER_INIT();
    
    /* UART Init*/
    CWM_UART_INIT();
    
    /* I2C Init*/
    CWM_I2C_INIT();

    CWM_SCREEN_INIT();
    CWM_GPS_INIT();
    
  
}

static void FreeRTOS_INIT(void)
{
    CWM_TASK1_INIT();
    CWM_TASK2_INIT();
}

int main(void)
{
    /*Need initial for first*/
    UTILITY_INIT();

    /* Platform Init*/
    PLATFORM_INIT();

    /* FreeRTOS Init*/
    FreeRTOS_INIT();

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    for (;;);

}

/**
  * @brief  Pre Sleep Processing
  * @param  ulExpectedIdleTime: Expected time in idle state
  * @retval None
  */
void PreSleepProcessing(uint32_t * ulExpectedIdleTime)
{
  /* Called by the kernel before it places the MCU into a sleep mode because
  configPRE_SLEEP_PROCESSING() is #defined to PreSleepProcessing().

  NOTE:  Additional actions can be taken here to get the power consumption
  even lower.  For example, peripherals can be turned off here, and then back
  on again in the post sleep processing function.  For maximum power saving
  ensure all unused pins are in their lowest power state. */

  /* 
    (*ulExpectedIdleTime) is set to 0 to indicate that PreSleepProcessing contains
    its own wait for interrupt or wait for event instruction and so the kernel vPortSuppressTicksAndSleep 
    function does not need to execute the wfi instruction  
  */
  *ulExpectedIdleTime = 0;
  
  /*Enter to sleep Mode using the HAL function HAL_PWR_EnterSLEEPMode with WFI instruction*/
#if 1
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);  
#else
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);
#endif
}

/**
  * @brief  Post Sleep Processing
  * @param  ulExpectedIdleTime: Not used
  * @retval None
  */
void PostSleepProcessing(uint32_t * ulExpectedIdleTime)
{
  /* Called by the kernel when the MCU exits a sleep mode because
  configPOST_SLEEP_PROCESSING is #defined to PostSleepProcessing(). */

  /* Avoid compiler warnings about the unused parameter. */
  (void) ulExpectedIdleTime;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 8;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
