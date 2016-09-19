#ifndef BOOT_DEBUG_H
#define BOOT_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "service_debug.h"


#define BOOT_DEBUG_SWITCH

#ifdef BOOT_DEBUG_SWITCH
#define BOOT_DEBUG        DEBUG_D
#else
#define BOOT_DEBUG
#endif


#ifdef __cplusplus
}
#endif


#endif //BOOT_MODE_H
