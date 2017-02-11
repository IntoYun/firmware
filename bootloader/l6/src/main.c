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
#include "system_config.h"
#include "boot_mode.h"
#include "params_hal.h"
#include "bkpreg_hal.h"
#include "boot_debug.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BOOTLOADER_VERSION  1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t USB_DFU_MODE          = 0;
uint8_t FACTORY_RESET_MODE    = 0;
uint8_t NC_MODE               = 0;
uint8_t START_APP_MODE        = 0;


/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
    HAL_System_Config();
    BOOT_DEBUG("boot start...\r\n");

    HAL_PARAMS_Load_Boot_Params();
    HAL_PARAMS_Load_System_Params();

    if(BOOTLOADER_VERSION != HAL_PARAMS_Get_Boot_boot_version())
    {
        BOOT_DEBUG("save boot version...\r\n");
        HAL_PARAMS_Set_Boot_boot_version(BOOTLOADER_VERSION);
        HAL_PARAMS_Save_Params();
    }

    switch(HAL_PARAMS_Get_Boot_boot_flag())
    {
        case BOOT_FLAG_NORMAL:          //正常启动
            START_APP_MODE = 1;
            break;
        case BOOT_FLAG_FACTORY_RESET:   //恢复出厂
            FACTORY_RESET_MODE = 1;
            break;
        case BOOT_FLAG_USB_DFU:         //进入DFU下载模式
            USB_DFU_MODE = 1;
            break;
        default:
            break;
    }

    //自动进入DFU下载模式
    if(0x7DEA == HAL_Core_Read_Backup_Register(BKP_DR_01))
    {
        USB_DFU_MODE = 1;
    }
    HAL_Core_Write_Backup_Register(BKP_DR_01, 0xFFFF);

    if(FACTORY_RESET_MODE)
    {
        BOOT_DEBUG("factroy reset\r\n");
        Enter_Factory_RESTORE_Mode();
        FACTORY_RESET_MODE = 0;
    }
    else if(USB_DFU_MODE)
    {
        BOOT_DEBUG("dfu\r\n");
        Enter_DFU_Mode();
    }
    else if(NC_MODE)
    {
        BOOT_DEBUG("reboot\r\n");
        System_Reset();
    }

    BOOT_DEBUG("start app\r\n");
    start_app();
    return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
