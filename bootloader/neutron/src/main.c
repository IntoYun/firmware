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
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction JumpToApplication;
uint32_t JumpAddress;

SDK_QUEUE USART_Rx_Queue;
SDK_QUEUE USB_Rx_Queue;

extern volatile uint32_t TimingBUTTON;
extern volatile uint32_t TimingLED;
extern RTC_HandleTypeDef RtcHandle;

extern boot_params_t intorobot_boot_params;  	//设备标识参数

uint8_t USB_DFU_MODE = 0;
uint8_t ESP8266_UPDATE_MODE = 0;
uint8_t ESP8266_COM_MODE = 0;
uint8_t DEFAULT_FIRMWARE_MODE = 0;
uint8_t OTA_FIRMWARE_MODE = 0;
uint8_t FACTORY_RESET_MODE = 0;
uint8_t NC_MODE = 0;
uint8_t ALL_RESET_MODE = 0;
uint8_t START_APP_MODE = 0;

void Start_App(void);
void Enter_DFU_Mode(void);
void Enter_ESP8266_Update_Mode();
void Enter_Default_Firmware_Update_Mode();
void Enter_ESP8266_Com_Mode(void);
void Enter_Factory_Update_Mode(char type);
void Enter_OTA_Firmware_Update_Mode(void);


extern void usartA2A3begin(uint32_t baud);
extern UART_HandleTypeDef UartHandleA;

extern void print(char *p);
extern void printn(char *p, int n);


#define BOOTLOADER_VERSION  1
#define LIGHTTIME 400
/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
    Set_System();
    Set_RGB_LED_Color(0);
    //usartA2A3begin(115200);
    Load_BootParams();

    if(BOOTLOADER_VERSION != intorobot_boot_params.boot_version)
    {
        intorobot_boot_params.boot_version = BOOTLOADER_VERSION;
        Save_BootParams();
    }

    if (BUTTON_GetState(FAC_BUTTON) == BUTTON_PRESSED)
    {
#define TIMING_DFU_DOWNLOAD_MODE     1000   //dfu 下载模式
#define TIMING_ESP8266_UPDATE_MODE   3000   //esp8266 升级判断时间
#define TIMING_DEFAULT_RESTORE_MODE  7000   //默认固件灯程序升级判断时间
#define TIMING_ESP8266_COM_MODE      10000  //esp8266串口转接判断时间
#define TIMING_FACTORY_RESET_MODE    13000  //恢复出厂程序判断时间 不清空密钥
#define TIMING_NC                    20000  //无操作判断时间
#define TIMING_ALL_RESET_MODE        30000  //完全恢复出厂判断时间 清空密钥
        TimingBUTTON = 0;
        while (BUTTON_GetState(FAC_BUTTON) == BUTTON_PRESSED)
        {
            if(TimingBUTTON>TIMING_ALL_RESET_MODE)
            {
                FACTORY_RESET_MODE = 0;
                ALL_RESET_MODE = 1;
                Set_RGB_LED_Color(RGB_COLOR_YELLOW);
            }
            else if(TimingBUTTON>TIMING_NC)
            {
                FACTORY_RESET_MODE = 0;
                NC_MODE = 1;
                Set_RGB_LED_Color(0);
            }
            else if(TimingBUTTON > TIMING_FACTORY_RESET_MODE)
            {
                ESP8266_COM_MODE = 0;
                FACTORY_RESET_MODE = 1;
                Set_RGB_LED_Color(RGB_COLOR_CYAN);
            }
            else if(TimingBUTTON > TIMING_ESP8266_COM_MODE)
            {
                DEFAULT_FIRMWARE_MODE = 0;
                ESP8266_COM_MODE = 1;
                Set_RGB_LED_Color(RGB_COLOR_BLUE);
            }
            else if(TimingBUTTON > TIMING_DEFAULT_RESTORE_MODE)
            {
                ESP8266_UPDATE_MODE = 0;
                DEFAULT_FIRMWARE_MODE = 1;
                Set_RGB_LED_Color(RGB_COLOR_GREEN);
            }
            else if(TimingBUTTON > TIMING_ESP8266_UPDATE_MODE)
            {
                USB_DFU_MODE = 0;
                ESP8266_UPDATE_MODE = 1;
                Set_RGB_LED_Color(RGB_COLOR_RED);
            }
            else if(TimingBUTTON > TIMING_DFU_DOWNLOAD_MODE)
            {
                USB_DFU_MODE = 1;
                Set_RGB_LED_Color(RGB_COLOR_MAGENTA);
            }
        }
    }
    else
    {
        switch(intorobot_boot_params.boot_flag)
        {
            case 0: //正常启动
                START_APP_MODE = 1;
                break;
            case 1: //默认程序下载
                DEFAULT_FIRMWARE_MODE = 1;
                break;
            case 2: //esp8266串口通讯
                ESP8266_COM_MODE = 1;
                break;
            case 3: //恢复出厂
                FACTORY_RESET_MODE = 1;
                break;
            case 4: //在线升级
                OTA_FIRMWARE_MODE = 1;
                break;
            case 5: //完全恢复出厂  清除密钥
                ALL_RESET_MODE = 1;
                break;
            case 6: //进入DFU下载模式
                USB_DFU_MODE = 1;
                break;
            default:
                break;
        }
    }
    //自动进入DFU下载模式
    if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1) == 0x7DEA)
    {
        USB_DFU_MODE = 1;
        HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0xFFFF);
    }

    if(ALL_RESET_MODE)
    {
        print("ALL factroy reset\r\n");
        Enter_Factory_Update_Mode(1);
        ALL_RESET_MODE = 0;
    }
    else if(FACTORY_RESET_MODE)
    {
        print("factroy reset\r\n");
        Enter_Factory_Update_Mode(0);
        FACTORY_RESET_MODE = 0;
    }
    else if(ESP8266_COM_MODE)
    {
        print("esp8266 com\r\n");
        Enter_ESP8266_Com_Mode();
    }
    else if(DEFAULT_FIRMWARE_MODE)
    {
        print("default firmware mode\r\n");
        Enter_Default_Firmware_Update_Mode();
        DEFAULT_FIRMWARE_MODE = 0;
    }
    else if(ESP8266_UPDATE_MODE)
    {
        print("esp8266 update\r\n");
        Enter_ESP8266_Update_Mode();
    }
    else if(USB_DFU_MODE)
    {
        print("dfu\r\n");
        Enter_DFU_Mode();
    }
    else if(OTA_FIRMWARE_MODE)
    {
        print("ota firmware\r\n");
        Enter_OTA_Firmware_Update_Mode();
        OTA_FIRMWARE_MODE = 0;
    }
    print("start app\r\n");
    Set_RGB_LED_Color(RGB_COLOR_RED); // color the same with atom
    delay(LIGHTTIME);
    Set_RGB_LED_Color(RGB_COLOR_GREEN); // color the same with atom
    delay(LIGHTTIME);
    Set_RGB_LED_Color(RGB_COLOR_BLUE); // color the same with atom
    delay(LIGHTTIME);
    Set_RGB_LED_Color(0);//防止进入应用程序初始化三色灯 导致闪灯

    Start_App();
    return 0;
}

void Start_App(void)
{
    if(((*(__IO uint32_t*)USBD_DFU_APP_DEFAULT_ADD) & 0x2FFE0000 ) == 0x20000000)
    {
        /* Jump to user application */
        JumpAddress = *(__IO uint32_t*) (USBD_DFU_APP_DEFAULT_ADD + 4);
        JumpToApplication = (pFunction) JumpAddress;

        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*) USBD_DFU_APP_DEFAULT_ADD);
        JumpToApplication();
    }
}

void Enter_DFU_Mode(void)
{
    Set_RGB_LED_Color(RGB_COLOR_MAGENTA);
    USBD_DFU_Init();
    while(1)
    {}
}

void Enter_ESP8266_Update_Mode(void)
{
    Set_RGB_LED_Color(RGB_COLOR_RED);
    Esp8266_Enter_UpdateMode();
    USBD_CDC_Init();
    while (1)
    {
        USBD_CDC_Process();
    }
}

void Enter_ESP8266_Com_Mode(void)
{
    Set_RGB_LED_Color(RGB_COLOR_BLUE);
    intorobot_boot_params.boot_flag = 0;
    Save_BootParams();
    USBD_CDC_Init();
    while (1)
    {
        USBD_CDC_Process();
    }
}

void Enter_Default_Firmware_Update_Mode(void)
{
    LED_SetSignalingColor(RGB_COLOR_YELLOW);
    LED_Signaling_Start();

    delay(4000);//8266启动必须延时2s以上 才能打开串口 否则会导致stm32死机 可能是串口中断问题 需要在验证。
    Uart1_Init();
    ESP8266_Init();

    DEFAULT_Flash_Reset();
    intorobot_boot_params.boot_flag = 0;
    Save_BootParams();
    LED_Signaling_Stop();
}

void Enter_Factory_Update_Mode(char type)
{
    LED_SetSignalingColor(RGB_COLOR_YELLOW);
    LED_Signaling_Start();

    delay(4000);//8266启动必须延时2s以上 才能打开串口 否则会导致stm32死机 可能是串口中断问题 需要在验证。
    Uart1_Init();
    ESP8266_Init();

    intorobot_boot_params.boot_flag = 0;
    if(type==0)
    {
        FACTORY_Flash_Reset();
        intorobot_boot_params.initparam_flag = 1;
    }
    else
    {
        intorobot_boot_params.initparam_flag = 2;
    }
    Save_BootParams();
}

void Enter_OTA_Firmware_Update_Mode(void)
{
    LED_SetSignalingColor(RGB_COLOR_YELLOW);
    LED_Signaling_Start();

    delay(4000);//8266启动必须延时2s以上 才能打开串口 否则会导致stm32死机 可能是串口中断问题 需要在验证。
    Uart1_Init();
    ESP8266_Init();

    OTA_Flash_Reset();
    intorobot_boot_params.boot_flag = 0;
    Save_BootParams();
    //HAL_NVIC_SystemReset();
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
