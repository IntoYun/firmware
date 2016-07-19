
#ifndef   DEVICE_CONFIG_HAL_H_
#define	  DEVICE_CONFIG_HAL_H_

#include <stdint.h>


/*
返回;0成功   1失败
*/
int mo_DeviceConfig_setWrtTimezone(float time_zone);


/*
返回:0需要配置
*/
int mo_DeviceConfig_isNeedDeviceConfig_hal(void);

/*
返回:0成功
*/
int mo_DeviceConfig_setDeviceConfig_hal(void);

/*
成功
*/
int mo_DeviceConfig_clearDeviceConfig_hal(void);




void mo_system_reboot_hal();

void mo_smartconfig_stop();
void mo_smartconfig_start();
void testNeutronPinOn(void);

void testNeutronPinOff(void);

#endif
