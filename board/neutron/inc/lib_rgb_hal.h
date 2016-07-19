

#ifndef   LIB_RGB_H_HAL_
#define   LIB_RGB_H_HAL_

#include <stdint.h>


void mo_RGBClass_off_hal(void);

void mo_RGBClass_color_hal(uint8_t red, uint8_t green, uint8_t blue);

void mo_RGBClass_blink_hal(uint8_t red, uint8_t green, uint8_t blue, uint16_t period);

void mo_RGBClass_breath_hal(uint8_t red, uint8_t green, uint8_t blue, uint16_t period);
void mo_RGBClass_hal();

#endif

