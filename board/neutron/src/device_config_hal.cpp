//=================================================================================================================
//input
/****************************************************************************
 *Private Included Files
 ****************************************************************************/

#include <stdint.h>
#include "lib_system_all.h"

#include "lib_wifi_drv.h"
#include "device_config_hal.h"
 //=================================================================================================================
//come true hidden
/************************************************************************************
* Private Types
************************************************************************************/


/************************************************************************************
* Private Variables
************************************************************************************/


/************************************************************************************
* Private Functions
************************************************************************************/


//=================================================================================================================
//come true export
/************************************************************************************
* Public Type
************************************************************************************/

extern RTC_HandleTypeDef RtcHandle;


/************************************************************************************
*  Public Functions
************************************************************************************/




/*
返回;0成功   1失败
*/
int mo_DeviceConfig_setWrtTimezone(float time_zone)
{
    MO_ERROR(("not support"));
    return 0;
}

/*
返回:0需要配置
*/
int mo_DeviceConfig_isNeedDeviceConfig_hal(void)
{
    MO_ERROR(("not support"));
    return 1;
}

/*
返回:0成功
*/
int mo_DeviceConfig_setDeviceConfig_hal(void)
{
    MO_ERROR(("not support"));
    return 0;
}

/*
成功
*/
int mo_DeviceConfig_clearDeviceConfig_hal(void)
{
    MO_ERROR(("not support"));
    return 0;
}

void mo_system_reboot_hal()
{
    MO_INFO(("reboot======================="));
    // HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR2, 0xA5A5);
    HAL_NVIC_SystemReset();
}

void mo_smartconfig_stop()
{
    mo_drv_wifi_run_cmd("AT+CWSTOPSMART","OK",10);
}

void mo_smartconfig_start()
{
    //设置为station
    mo_drv_wifi_run_cmd("AT+CWMODE_DEF=1","OK",10);
    mo_drv_wifi_run_cmd("AT+CWDHCP_DEF=1,1","OK",10);
    mo_drv_wifi_run_cmd("AT+CWSTOPSMART","OK",10);

    mo_drv_wifi_run_cmd("AT+CWSTARTSMART=1","smartconfig connected",0);
#if 0
    if( mo_drv_wifi_run_cmd("AT+CWSTARTSMART=1","smartconfig connected",600))
    {
      if(mo_drv_wifi_run_cmd("AT+CWSTOPSMART","OK",10))
      {
        MO_INFO(("OK SMART CONFIG"));
      }
    }
#endif
}

#define NUMALLPIN 16
void testNeutronPinOn(void)
{
  int allPin[NUMALLPIN] = {D0, D1, D2, D3, D4, D5, D6, D7, A0, A1, A2, A3, A4, A5, A6, A7};
  for (int i = 0; i < NUMALLPIN; i++)
  {
    pinMode(allPin[i], OUTPUT);
  }
  for (int i = 0; i < NUMALLPIN; i++)
  {
    digitalWrite(allPin[i], HIGH);
  }

}

void testNeutronPinOff(void)
{
  int allPin[NUMALLPIN] = {D0, D1, D2, D3, D4, D5, D6, D7, A0, A1, A2, A3, A4, A5, A6, A7};
  for (int i = 0; i < NUMALLPIN; i++)
  {
    pinMode(allPin[i], OUTPUT);
  }
  for (int i = 0; i < NUMALLPIN; i++)
  {
    digitalWrite(allPin[i], LOW);
  }
}
