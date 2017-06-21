/**
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
*/

#ifndef LORA_GATEWAY_UI_H_
#define LORA_GATEWAY_UI_H_

#include "lora_gateway_config.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    LED_MODE_COLOR = 0,
    LED_MODE_BLINK,
    LED_MODE_BREATH
}led_mode_t;

typedef struct {
    led_mode_t  led_mode;
    uint8_t     led_value;
    uint16_t    led_period;
}led_info_t;

typedef enum
{
    BUTTON_MODE_NONE = 0,
    BUTTON_MODE_CONFIG_IMLINK_SERIAL,
    BUTTON_MODE_CONFIG_AP_SERIAL,
    BUTTON_MODE_FAC,
} button_mode_t;

void gatewayUiInitial(void);
uint8_t gatewayUiModeButtonGetState(void);
uint32_t gatewayUiModeButtonPressed(void);
void gatewayUiIndicatorToggle(void);
void gatewayUiIndicatorControl(uint8_t value);
void gatewayUiIndicatorBlink(uint16_t period);
void gatewayUiSysTickHandler(void);
void gatewayUiModeSysTickHandler(void);

#ifdef __cplusplus
}
#endif

#endif

