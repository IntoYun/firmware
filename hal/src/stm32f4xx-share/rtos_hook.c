#include "concurrent_hal.h"


void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ) {
    //PANIC(StackOverflow, "Stack overflow detected");
}

