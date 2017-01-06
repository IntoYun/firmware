#include "concurrent_hal.h"
#include "FreeRTOS.h"
#include "task.h"

extern "C" {
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ) {
    //PANIC(StackOverflow, "Stack overflow detected");
}
} // extern "C"

