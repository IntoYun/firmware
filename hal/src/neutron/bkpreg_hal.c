#include "core_hal.h"
#include "hw_config.h"

extern RTC_HandleTypeDef RtcHandle;

int32_t HAL_Core_Backup_Register(uint32_t BKP_DR)
{
    switch (BKP_DR)
    {
    case BKP_DR_01:
        return RTC_BKP_DR1;
        break;
    case BKP_DR_10:
        return RTC_BKP_DR10;
        break;
    }
    return -1;
}

void HAL_Core_Write_Backup_Register(uint32_t BKP_DR, uint32_t Data)
{
    uint32_t BKP_DR_Address = HAL_Core_Backup_Register(BKP_DR);
    if (BKP_DR_Address != -1)
    {
        HAL_RTCEx_BKUPWrite(&RtcHandle, BKP_DR_Address, Data);
    }
}

uint32_t HAL_Core_Read_Backup_Register(uint32_t BKP_DR)
{
    uint32_t BKP_DR_Address = HAL_Core_Backup_Register(BKP_DR);
    if (BKP_DR_Address != -1)
    {
        return HAL_RTCEx_BKUPRead(&RtcHandle, BKP_DR_Address);
    }
    return 0xFFFFFFFF;
}


