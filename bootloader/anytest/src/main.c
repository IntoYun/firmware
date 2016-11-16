/**
******************************************************************************
* @file    USB_Device/DFU_Standalone/Src/main.c
* @author  MCD Application Team
* @version V1.3.2
* @date    13-November-2015
* @brief   USB device DFU demo main file
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "boot_mode.h"
#include "core_hal.h"
#include "boot_debug.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
    HAL_System_Config();
    BOOT_DEBUG("boot start...\r\n");

    start_app();
    return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
