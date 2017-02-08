/*该头文件引用由IntoRobot自动添加.*/
#include "Adafruit_SSD1306.h"

/*
************************************************************************
* 作者:  IntoRobot Team 
* 版本:  V1.0.0
* 版权：Copyright (c) 2016 Shenzhen MOLMC Technology Co., Ltd..
* 日期:  Dec. 21 2016
*
* The MIT License (MIT)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
IntoBike: 1.电池电压检测; 2.OLED屏幕的显示.
描述：
1. 初始化后显示IntoRobot的LoGo;
2. 第一行显示IntoBike;
3. 第二行显示电池电压，如果出现低电压，则整个屏幕闪烁，提示电压过低;
4. 第三行显示左轮和右轮速度;
5. 第四行显示倾角和角速度
6. 第五行显示障碍物距离。
*/
#ifndef __INTOBIKE_OLEDDISPLAY_
#define __INTOBIKE_OLEDDISPLAY_
#include "application.h"
/*该头文件引用由IntoRobot自动添加.*/
#include "IntoBike_State_Machine.h"
/*该头文件引用由IntoRobot自动添加.*/
#include "MeanFilter.h"
/*该头文件引用由IntoRobot自动添加.*/
#include "IntoBike_Balance_Control.h"
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
#define VOLTAGE_AD_PIN A7
#define LCD_HEIGHT 64
#define LCD_WIDTH  128
// static uint8_t intorobot_logo_bmp[LCD_HEIGHT * LCD_WIDTH / 8] =
// {
// 0
// };

struct DisplayData
{
	float voltage_;
	float velocity_l_;
	float velocity_r_;
	float obstacle_distance_;
	float pitch_;
	float pitch_rate_;
	float control_l_;
	float control_r_;
}
;
class IntoBikeOledDisplay: Adafruit_SSD1306
{
	public:
	IntoBikeOledDisplay();
	void init();
	void begin();
	void begin(uint8_t i2caddr);
	void displayData(u8 flag);
	void displayData(void);
	void setData(DisplayData data);
	void setData(DisplayDebugInfo data);
	void setData(StateMachineInfo data);
	void switchDisplay(char command);
	void printStateEvent(IntoBikeState state, IntoBikeEvent event);
	void displayLogo();
	void readVoltage();
	private:
	DisplayData       data_;
	DisplayDebugInfo  debug_data_;
	StateMachineInfo  statemachine_data_;
	MeanFilter        lowpass_filter_;
	uint8_t           display_index_ = 0;
	char              curve_data_[LCD_WIDTH];
}
;
#endif