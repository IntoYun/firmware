#include "boot_mode.h"
#include "memory_hal.h"
#include "params_hal.h"
#include "ui_hal.h"
#include "flash_map.h"
#include "esp8266_comm.h"
#include "system_config.h"
#include "boot_debug.h"

extern UART_HandleTypeDef UartHandleEsp8266;
extern SDK_QUEUE USART_Esp8266_Queue;

pFunction JumpToApplication;
uint32_t JumpAddress;

void start_app(void)
{
    if(((*(__IO uint32_t*)APP_ADDR) & 0x2FFE0000 ) == 0x20000000)
    {
        /* Jump to user application */
        JumpAddress = *(__IO uint32_t*) (APP_ADDR + 4);
        JumpToApplication = (pFunction) JumpAddress;

        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*) APP_ADDR);
        JumpToApplication();
    }
}

static void FLASH_Erase_Impl(uint32_t address, uint32_t length)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t FirstSector = 0, NbOfSectors = 0, SECTORError = 0;

    HAL_FLASH_Unlock();
    /* Get the 1st sector to erase */
    FirstSector = HAL_FLASH_Interminal_Get_Sector(address);
    /* Get the number of sector to erase from 1st sector*/
    NbOfSectors = HAL_FLASH_Interminal_Get_Sector(address + length) - FirstSector + 1;
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FirstSector;
    EraseInitStruct.NbSectors = NbOfSectors;
    HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
    HAL_FLASH_Lock();
}

bool FLASH_Restore(Firmware_TypeDef FmType)
{
    #define PACKAGE_UNIT 16384  //16k
    int PacketNum,Count;

    PacketNum=ESP8266_Firmware_Size(FmType, PACKAGE_UNIT);
    if(PacketNum)
    {
        BOOT_DEBUG("erase begin\r\n");
        FLASH_Erase_Impl(APP_ADDR, PacketNum * PACKAGE_UNIT) ;
        BOOT_DEBUG("erase over\r\n");
        for(Count = 0; Count < PacketNum; Count++)
        {
            if(ESP8266_RSP_SUCCESS != ESP8266_Firware_Packet(FmType, PACKAGE_UNIT, Count, 1000))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

void USBD_DFU_Init(void)
{
    USBD_Init(&USBD_Device, &DFU_Desc, 0);
    USBD_RegisterClass(&USBD_Device, USBD_DFU_CLASS);
    USBD_DFU_RegisterMedia(&USBD_Device, &USBD_DFU_fops);
    USBD_Start(&USBD_Device);
}

void USBD_CDC_Init(void)
{
    USBD_Init(&USBD_Device, &VCP_Desc, 0);
    USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
    USBD_Start(&USBD_Device);
}

void USBD_CDC_Process(void)
{
    uint8_t TxBuffer[1024];
    uint32_t len=0;

    len=0;
    while(!sdkIsQueueEmpty(&USART_Esp8266_Queue))
    {
        if(len < sizeof(TxBuffer))
        {
            sdkGetQueueData(&USART_Esp8266_Queue, &TxBuffer[len++]);
        }
        else
        {
            break;
        }
    }
    if(len)
    {
        USBD_CDC_SetTxBuffer(&USBD_Device, TxBuffer, len);
        while(USBD_CDC_TransmitPacket(&USBD_Device) != USBD_OK);
    }

    len=0;
    while(!sdkIsQueueEmpty(&USB_Rx_Queue))
    {
        if(len < sizeof(TxBuffer))
        {
            sdkGetQueueData(&USB_Rx_Queue, &TxBuffer[len++]);
        }
        else
        {
            break;
        }
    }
    if(len)
    {
        HAL_UART_Transmit(&UartHandleEsp8266, TxBuffer, len, 2000);//2000ms
    }
}

bool DEFAULT_Flash_Reset(void)
{
    return FLASH_Restore(DEFAULT_FIRWARE);
}

bool OTA_Flash_Reset(void)
{
    return FLASH_Restore(OTA_FIRWARE);
}

void Enter_Default_RESTORE_Mode(void)
{
    HAL_UI_RGB_Blink(RGB_COLOR_YELLOW, 100);
    ESP8266_Init();
    if(DEFAULT_Flash_Reset())
    {
        HAL_PARAMS_Set_Boot_boot_flag(0);
        HAL_PARAMS_Save_Params();
    }
    else
    {
        System_Reset();
    }
}

void Enter_Serail_Com_Mode(void)
{
    HAL_UI_RGB_Color(RGB_COLOR_BLUE);
    HAL_PARAMS_Set_Boot_boot_flag(0);
    HAL_PARAMS_Save_Params();
    USBD_CDC_Init();
    while (1)
    {
        USBD_CDC_Process();
    }
}

void Enter_Factory_RESTORE_Mode(void)
{
    HAL_UI_RGB_Blink(RGB_COLOR_YELLOW, 100);
    ESP8266_Init();
    if(DEFAULT_Flash_Reset())
    {
        HAL_PARAMS_Set_Boot_initparam_flag(1);
        HAL_PARAMS_Set_Boot_boot_flag(0);
        HAL_PARAMS_Save_Params();
    }
    else
    {
        System_Reset();
    }
}

void Enter_Factory_ALL_RESTORE_Mode(void)
{
    HAL_UI_RGB_Blink(RGB_COLOR_YELLOW, 100);
    delay(1000);
    HAL_PARAMS_Set_Boot_initparam_flag(2);
    HAL_PARAMS_Set_Boot_boot_flag(0);
    HAL_PARAMS_Save_Params();
}

void Enter_OTA_Update_Mode(void)
{
    HAL_UI_RGB_Blink(RGB_COLOR_YELLOW, 100);
    ESP8266_Init();
    if(OTA_Flash_Reset())
    {
        HAL_PARAMS_Set_Boot_boot_flag(0);
        HAL_PARAMS_Save_Params();
    }
    else
    {
        System_Reset();
    }
}

void Enter_DFU_Mode(void)
{
    HAL_UI_RGB_Color(RGB_COLOR_MAGENTA);
    USBD_DFU_Init();
    while(1)
    {}
}

void Enter_ESP8266_Update_Mode(void)
{
    HAL_UI_RGB_Color(RGB_COLOR_RED);
    Esp8266_Enter_UpdateMode();
    USBD_CDC_Init();
    while (1)
    {
        USBD_CDC_Process();
    }
}

