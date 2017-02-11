#ifndef  WIRING_EX_H_
#define  WIRING_EX_H_

#include "wiring_ex_sensor.h"

//为了兼容旧版本neutron固件，之前动态申请使用freertos接口。否则用户的程序会编译错误
#define pvPwortMalloc   malloc
#define vPortFree       free

#endif

