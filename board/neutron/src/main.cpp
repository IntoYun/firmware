/**
******************************************************************************
* @file    GPIO/GPIO_IOToggle/Src/main.c
* @author  MCD Application Team
* @version V1.2.3
* @date    13-November-2015
* @brief   This example describes how to configure and use GPIOs through
*          the STM32F4xx HAL API.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

#include "lib_wifi_drv.h"
#include "intorobot_api.h"


/** @addtogroup STM32F4xx_HAL_Examples
 * @{
 */

/** @addtogroup GPIO_IOToggle
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

osThreadId LEDThread1Handle;
osThreadId handle_setup_loop;


/* Private function prototypes -----------------------------------------------*/


void setup() __attribute__ ((weak));
void loop() __attribute__ ((weak));
extern void RTC_Init();

static void task_setup_loop(void const *argument)
{
  delay(2000);

  if(NULL != setup)
  {
    setup();
  }

  if(NULL != loop)
  {
    for(;;)
    {
      loop();
    }
  }


}


void mo_setup_loop_init()
{
  //setup_loop_init
  osThreadDef(SETUP_LOOP, task_setup_loop, osPriorityNormal, 0, 1024);
  handle_setup_loop = osThreadCreate(osThread(SETUP_LOOP),NULL);
  MO_ASSERT((handle_setup_loop!=NULL));
}



/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
  while(1)
  {
  }
}


/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 96000000
 *            HCLK(Hz)                       = 96000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 2
 *            APB2 Prescaler                 = 1
 *            HSI Frequency(Hz)              = 16000000
 *            HSE Frequency(Hz)              = 26000000
 *            PLL_M                          = 13
 *            PLL_N                          = 96
 *            PLL_P                          = 2
 *            PLL_Q                          = 4
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale2 mode
 *            Flash Latency(WS)              = 4
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 13;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}



#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

void mo_display_start()
{

  MO_INFO(("====================Compile Time:%s===================",__TIME__));
  MO_INFO(("                        \\\\\\|///                             "));
  MO_INFO(("                      \\\\  - -  //                           "));
  MO_INFO(("                       (  @ @  )                            "));
  MO_INFO((" ____________________oOOo_(.)_oOOo_________________________ "));
  MO_INFO(("|                                                          |"));
  MO_INFO(("|             Welcome to IntoRobot  System                 |"));
  MO_INFO(("|                              Oooo                        |"));
  MO_INFO(("|______________________ oooO___(   )_______________________|"));
  MO_INFO(("                       (   )    ) /                         "));
  MO_INFO(("                        \\ (    (_/                          "));
  MO_INFO(("                         \\_)                                "));

}


void mo_init()
{
    DWT_Init();
    RTC_Init();
    SerialUSB.begin();
    Serial.begin();
	mo_display_start();
}



/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
    /* This sample code shows how to use STM32F4xx GPIO HAL API to toggle PA05 IOs
     connected to LED2 on STM32F4xx-Nucleo board
     in an infinite loop.
     To proceed, 3 steps are required: */

    /* STM32F4xx HAL library initialization:
     - Configure the Flash prefetch, instruction and Data caches
     - Configure the Systick to generate an interrupt each 1 msec
     - Set NVIC Group Priority to 4
     - Global MSP (MCU Support Package) initialization
    */
    HAL_Init();

    /* Configure the system clock to 96 MHz */
    SystemClock_Config();

	//===========================================
    mo_init();
    // osThreadDef(RGBLED, LED_Thread1, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    // LEDThread1Handle = osThreadCreate(osThread(RGBLED), NULL);
    // MO_ASSERT((LEDThread1Handle!=NULL));
    mo_drv_wifi_init();  // Create the task uart0 filter
    mo_intorobot_init(); // intorobot_setup_loop task
    // rgb_light and the button check task in the RGBClass::RGBClass mo_RGBClass_hal();

	//===========================================


    osKernelStart();

    for(;;);

}


