#include "concurrent_hal.h"


#ifdef DEBUG_BUILD
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ) {
    //PANIC(StackOverflow, "Stack overflow detected");
}
#endif

