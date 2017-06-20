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
#include "lora_gateway_ui.h"
#include "lora_gateway_log.h"
#include "sx1278-board.h"


#if PLATFORM_ID == PLATFORM_GL1000
#define MODE_BUTTON_PIN             D9    //定义按键引脚
#define INDICATOR_LED_PIN           D8    //定义指示灯引脚
// 定义网关管脚连接
const lora_pinmap_t lora_pins = {
    .nss  = A4,
    .rxtx = D6,
    .rst  = D7,
    .dio  = {D0, D1, D2, D3, LORA_UNUSED_PIN, LORA_UNUSED_PIN},
};
#else
#define MODE_BUTTON_PIN             GPIO0     //定义按键引脚
#define INDICATOR_LED_PIN           GPIO16    //定义指示灯引脚
// 定义网关管脚连接
const lora_pinmap_t lora_pins = {
    .nss  = GPIO15,
    .rxtx = GPIO5,
    .rst  = GPIO2,
    .dio  = {GPIO4, LORA_UNUSED_PIN, LORA_UNUSED_PIN, LORA_UNUSED_PIN, LORA_UNUSED_PIN, LORA_UNUSED_PIN},
};
#endif

static bool BUTTON_last_state = 0;
static int TimingBUTTON = 0;
static int TimingLED = 0;
static led_info_t led_info;
static button_mode_t BUTTON_Mode = BUTTON_MODE_NONE;
static int BUTTON_press_time;

static void processTimerISR(void);

Timer _process_timer(100, processTimerISR);

static void processTimerISR(void)
{
    gatewayUiSysTickHandler();
    gatewayUiModeSysTickHandler();
}

void gatewayUiInitial(void)
{
    memset(&led_info, 0, sizeof(led_info));

    pinMode(MODE_BUTTON_PIN, INPUT);
    pinMode(INDICATOR_LED_PIN, OUTPUT);
    gatewayUiIndicatorControl(false);
    _process_timer.start();
}

uint8_t gatewayUiModeButtonGetState(void)
{
    return digitalRead(MODE_BUTTON_PIN);
}

uint32_t gatewayUiModeButtonPressed(void)
{
    return TimingBUTTON;
}

void gatewayUiIndicatorToggle(void)
{
    if(led_info.led_value) {
        digitalWrite(INDICATOR_LED_PIN, LOW);
        led_info.led_value = 0;
    } else {
        digitalWrite(INDICATOR_LED_PIN, HIGH);
        led_info.led_value = 1;
    }
}

void gatewayUiIndicatorControl(uint8_t value)
{
    if(value) {
        digitalWrite(INDICATOR_LED_PIN, LOW);
    } else {
        digitalWrite(INDICATOR_LED_PIN, HIGH);
    }
    led_info.led_mode = LED_MODE_COLOR;
}

void gatewayUiIndicatorBlink(uint16_t period)
{
    led_info.led_mode = LED_MODE_BLINK;
    led_info.led_value = 0;
    led_info.led_period = period > 100 ? period >> 1 : 50;
}

void gatewayUiSysTickHandler(void)
{
    //指示灯闪灯处理
    if(LED_MODE_BLINK == led_info.led_mode) {
        if (TimingLED > 0) {
            TimingLED -= 100;
        } else {
            gatewayUiIndicatorToggle();
            TimingLED = led_info.led_period;
        }
    }
    //模式按键处理 按键已按下
    if(!gatewayUiModeButtonGetState()) {
        if(!BUTTON_last_state) {
            TimingBUTTON = 0;
            BUTTON_last_state = 1;
        } else {
            TimingBUTTON += 100;
        }
    } else {
        if(BUTTON_last_state) {
            TimingBUTTON = 0;
            BUTTON_last_state = 0;
        }
    }
}

void gatewayUiModeSysTickHandler(void)
{
#define TIMING_MODE_CONFIG_IMLINK_SERIAL           3000    //进入imlink配置模式判断时间
#define TIMING_MODE_CONFIG_AP_SERIAL               5000    //进入ap配置模式判断时间
#define TIMING_MODE_FACTORY_RESET                  10000   //恢复出厂程序判断时间

    BUTTON_press_time = gatewayUiModeButtonPressed();
    if(BUTTON_press_time) {
        if( BUTTON_press_time > TIMING_MODE_FACTORY_RESET ) {
            if(BUTTON_Mode!=BUTTON_MODE_FAC) {
                BUTTON_Mode=BUTTON_MODE_FAC;       //恢复出厂设置
                gatewayUiIndicatorControl(true);
            }
        }
        else if( BUTTON_press_time > TIMING_MODE_CONFIG_IMLINK_SERIAL ) {
            if(BUTTON_Mode!=BUTTON_MODE_CONFIG_IMLINK_SERIAL) {
                BUTTON_Mode=BUTTON_MODE_CONFIG_IMLINK_SERIAL; //wifi imlink配置模式
                gatewayUiIndicatorBlink(500);
            }
        }
    } else {
        switch(BUTTON_Mode) {
            case BUTTON_MODE_CONFIG_IMLINK_SERIAL:
                System.configEnterMode(SYSTEM_CONFIG_TYPE_IMLINK_SERIAL);
                break;
            case BUTTON_MODE_FAC:
                System.factoryReset();
                break;
            default:
                break;
        }
        if(BUTTON_MODE_NONE != BUTTON_Mode) {
            BUTTON_Mode = BUTTON_MODE_NONE;
        }
    }
}

