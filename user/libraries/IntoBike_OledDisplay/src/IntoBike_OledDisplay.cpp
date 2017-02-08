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
#include "IntoBike_OledDisplay.h"
extern uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8];
IntoBikeOledDisplay::IntoBikeOledDisplay() : Adafruit_SSD1306(D6)
{
	init();
}
void IntoBikeOledDisplay::begin()
{
	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
	begin(0x78>>1);	// initialize with the I2C addr 0x78 (for the 128x64), it should be shifted left a bit for our oled
}
void IntoBikeOledDisplay::begin(uint8_t i2caddr)
{
	// by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
	Adafruit_SSD1306::begin(SSD1306_SWITCHCAPVCC, i2caddr);
	displayLogo();
}
void IntoBikeOledDisplay::init()
{
	data_.voltage_           = 1000.0f;
	lowpass_filter_.input(data_.voltage_);	//initialize the filter
	data_.velocity_l_        = 0.0f;
	data_.velocity_r_        = 0.0f;
	data_.obstacle_distance_ = 0.0f;
	data_.pitch_             = 0.0f;
	data_.pitch_rate_        = 0.0f;
	for (int i=0; i<LCD_WIDTH; i++)
	curve_data_[i] = -1;
	pinMode(VOLTAGE_AD_PIN, AN_INPUT);
}
void IntoBikeOledDisplay::displayLogo()
{
	display();
	delay(2000);
}
void IntoBikeOledDisplay::displayData(u8 flag)
{
	readVoltage();	//update voltage
	if(data_.voltage_ < 11.1)   //safe Li battery voltage
	{
		setTextSize(2);
		setTextColor(WHITE);
		setCursor(0,0);
		clearDisplay();
		println("-Warning-\n");
		println("LowVoltage\n");
		printf("Vol: %3.2f\n", data_.voltage_);
	}
	else
	{
		setTextSize(1);
		setTextColor(WHITE);
		setCursor(0,0);
		clearDisplay();
		switch(flag)
		{
			case 0:
			{
				println("------IntoBike------\n");
				printf("Vol:%3.2f  %3.2f%%\n", data_.voltage_, (data_.voltage_ - 10.8) / (12.6 - 10.8) * 100);
				printf("VL :%4.2f\nVR: %4.2f\n", data_.velocity_l_, data_.velocity_r_);
				printf("Pc :%3.2f-Pr:%3.2f\n", data_.pitch_, data_.pitch_rate_);
				printf("Distance:%3.2f\n", data_.obstacle_distance_);
				break;
			}
			case 1:
			{
				printf("BK:%.1f %.1f\n",       debug_data_.control_params_.bc_kp_,  debug_data_.control_params_.bc_kd_);
				printf("VK:%.4f %.4f\n",       debug_data_.control_params_.vc_kp_,  debug_data_.control_params_.vc_ki_);
				printf("VL:%4.0f %4.0f\n",     debug_data_.wheels_command_speed_.left_, debug_data_.wheels_command_speed_.right_);
				printf("VIng:%.0f\n",          debug_data_.vc_iterm_);
				printf("C:%.0f %.0f %.0f\n",   debug_data_.balance_pwm_, debug_data_.velocity_pwm_, debug_data_.rotation_pwm_);
				printf("Filter:%d\n",          debug_data_.filter_type_);
				printf("S: %3.2f %3.2f\n",     debug_data_.pitch_, debug_data_.pitch_rate_);
				printf("Vol:%3.2f  %3.2f%%\n", data_.voltage_, (data_.voltage_ - 10.8) / (12.6 - 10.8) * 100);
				break;
			}
			case 2:
			{
				printStateEvent(statemachine_data_.state_, statemachine_data_.event_);
				printf("a_z:%4.2f\n",         statemachine_data_.data_.accel_z_);
				printf("dist:%4.2f\n",        statemachine_data_.data_.obstacle_distance_);
				printf("Pc:%3.2f-Pr:%3.2f\n", statemachine_data_.data_.pitch_, statemachine_data_.data_.pitch_rate_);
				printf("EL:%3.2f-ER:%3.2f\n", statemachine_data_.data_.encoder_left_, statemachine_data_.data_.encoder_right_);
				break;
			}
			case 3:
			{
				int i=0, j=0;
				for (i=0; i<LCD_WIDTH; i++)
				{
					j++;
					if(curve_data_[i] == -1)
					{
						curve_data_[i] = (int8_t)((data_.pitch_ + 90) / 180.0 * 64);
						break;
					}
				}
				if(j == LCD_WIDTH)
				{
				    for (i=0; i<LCD_WIDTH - 1; i++)
				    {
				        curve_data_[i] = curve_data_[i+1];
				    }
				    curve_data_[LCD_WIDTH - 1] = (int8_t)((data_.pitch_ + 90) / 180.0 * 64);
				}
				for (i=1; i<j; i++)
				{
					drawPixel(i, curve_data_[i], WHITE);
					drawLine(i-1, curve_data_[i-1], i, curve_data_[i], WHITE);
				}
			}
		}
	}
	display();
}
void IntoBikeOledDisplay::printStateEvent(IntoBikeState state, IntoBikeEvent event)
{
	printf("State:%d ",   state);
	switch(state)
	{
		case BLOCKED:
		{
			printf("BLOCKED\n");
			break;
		}
		case IDLE:
		{
			printf("IDLE\n");
			break;
		}
		case REMOTE_CONTROLED:
		{
			printf("REMOTE_CONTROLED\n");
			break;
		}
		case STOP:
		{
			printf("STOP\n");
			break;
		}
		case FALL_DOWN:
		{
			printf("FALL_DOWN\n");
			break;
		}
		case PICKED_UP:
		{
			printf("PICKED_UP\n");
			break;
		}
	}
	printf("Event:%d ",  event);
	switch(event)
	{
		case BLOCKED_EVENT:
		{
			printf("BLOCKED\n");
			break;
		}
		case UNBLOCKED_EVENT:
		{
			printf("UNBLOCKED\n");
			break;
		}
		case REMOTE_CONTROLED_EVENT:
		{
			printf("CONTROLED\n");
			break;
		}
		case OUTOFCONTROL_EVENT:
		{
			printf("OUTOFCONTROL\n");
			break;
		}
		case FALL_DOWN_EVENT:
		{
			printf("FALLDOWN\n");
			break;
		}
		case STAND_UP_EVENT:
		{
			printf("STANDUP\n");
			break;
		}
		case PICKED_UP_EVENT:
		{
			printf("PICKEDUP\n");
			break;
		}
		case PUT_DOWN_EVENT:
		{
			printf("PUTDOWN\n");
			break;
		}
	}
}
//command from bluetooth
void IntoBikeOledDisplay::switchDisplay(char command)
{
	if(command == 'd')
	{
		display_index_++;
		if(display_index_ > 3)
		{
			display_index_ = 0;
		}
	}
}
void IntoBikeOledDisplay::displayData()
{
	displayData(display_index_);
}
//@params flag 0-normal data, 1-debug params, 2-statemachine info
void IntoBikeOledDisplay::setData(DisplayData data)
{
	data_ = data;
}
//@params flag 0-normal data, 1-debug params, 2-statemachine info
void IntoBikeOledDisplay::setData(DisplayDebugInfo data)
{
	debug_data_ = data;
}
//@params flag 0-normal data, 1-debug params, 2-statemachine info
void IntoBikeOledDisplay::setData(StateMachineInfo data)
{
	statemachine_data_ = data;
}
void IntoBikeOledDisplay::readVoltage()
{
	data_.voltage_ = (float)(analogRead(VOLTAGE_AD_PIN)) * 0.0045922 / 0.9854 * 0.9806;	/// 3.3 / 10.0 * 57.0 / 4096.0 = 0.0045922, Here 0.9854 is a calibrated param
	if(data_.voltage_ > 1.0)
	    data_.voltage_ = lowpass_filter_.input(data_.voltage_);
}