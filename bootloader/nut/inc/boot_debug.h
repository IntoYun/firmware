#include "hw_config.h"


//#define BOOT_DEBUG_SWITCH

#ifdef BOOT_DEBUG_SWITCH
#define BOOT_DEBUG        ets_printf
#else
#define BOOT_DEBUG
#endif

