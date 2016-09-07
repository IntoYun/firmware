#include "hw_config.h"


extern void debug_print(char *p);


//#define BOOT_DEBUG_SWITCH

#ifdef BOOT_DEBUG_SWITCH
#define BOOT_DEBUG        debug_print
#else
#define BOOT_DEBUG
#endif

