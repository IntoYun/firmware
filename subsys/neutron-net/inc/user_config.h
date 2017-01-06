#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#include "eboot_command.h"
#include "at_ex_command.h"
#include "upgrade.h"

/*版本号说明  前三位 1.0.0 esp8266版本号 第4位 为stm32 bootloader版本号，该位不要轻易变动*/
//#define  WIFIVERSION               "1.0.2.1"
#define  WIFIVERSION               "2.0.0.1"  //sdk 更新至2.0.0

//#define DEBUG_BUILD

#ifdef DEBUG_BUILD
#define DEBUG os_printf
#else
#define DEBUG
#endif

extern struct eboot_command cmd_info;

#endif
