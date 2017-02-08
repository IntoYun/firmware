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
描述：
IntoBike与IntoRobotCloud进行交互的类。主要是从IntoRobotCloud中获取控制参数和控制指令。
具体的参数包括：
1. 小车平衡控制的PD参数；
2. 小车速度控制的PI参数；
3. 小车方向控制的PD参数。
控制指令包括：
1. 前进后退；
2. 左右转弯；
3. 停止。
*/

#include "IntoBike_IntoRobotCloud_Interface.h"

IntoBikeCloudInterface* IntoBikeCloudInterface::m_singleton = new IntoBikeCloudInterface();// 指向单例对象的指针
IntoBikeCloudInterface* _intobike_cloud_interface_ptr = IntoBikeCloudInterface::getInstance();

WidgetJoypad _joypad;
#ifdef __cplusplus
extern "C"
{
#endif 
	void paramsCallback(uint8_t * payload, uint32_t len)
	{
		BalanceControlParams params;
		String  str;
		if(jsonGetValue(payload,"switchEx",str))
		{
			if(str.toInt() == 0)
			{
				_intobike_cloud_interface_ptr->stepmotor_control_[0].disableMotors();
				_intobike_cloud_interface_ptr->stepmotor_control_[1].disableMotors();
				IntoRobot.printf("test1");
			}
			else
			{
				_intobike_cloud_interface_ptr->stepmotor_control_[0].enableMotors();
				_intobike_cloud_interface_ptr->stepmotor_control_[1].enableMotors();
				IntoRobot.printf("test2");
			}
		}
		if(jsonGetValue(payload,"sliderbar_BP",str))
		{
			params.bc_kp_ = (float)str.toInt()/1.0;
		}
		if(jsonGetValue(payload,"sliderbar_BD",str))
		{
			params.bc_kd_ = (float)str.toInt()/10.0;
		}
		if(jsonGetValue(payload,"sliderbar_VP",str))
		{
			params.vc_kp_ = (float)str.toInt()/100.0;
		}
		if(jsonGetValue(payload,"sliderbar_VI",str))
		{
			params.vc_ki_ = (float)str.toInt()/5000.0;
			//params.vc_ki_ = params.vc_kp_ / 100.0;
		}
		if(jsonGetValue(payload,"max_magnitude_sliderbar",str))
		{
			params.max_magnitude_ = str.toInt();
		}
		if(jsonGetValue(payload,"pwm_max_sliderbar",str))
		{
			_intobike_cloud_interface_ptr->stepmotor_control_[0].setMaxSpeed(str.toInt());
			_intobike_cloud_interface_ptr->stepmotor_control_[1].setMaxSpeed(str.toInt());
		}
		_intobike_cloud_interface_ptr->balance_controller_->setParams(params);
	}
#ifdef __cplusplus
}
#endif 
void IntoBikeCloudInterface::begin(BalanceControl *balance_controller, StepMotorControl stepmotor_control[])
{
	balance_controller_ = balance_controller;
	stepmotor_control_  = stepmotor_control;
	IntoRobot.subscribe(BALANCE_CONTROL,NULL, paramsCallback);
    _joypad.begin(controlCallback);
}
void IntoBikeCloudInterface::begin(StepMotorControl stepmotor_control[], BalanceControl *balance_controller)
{
	balance_controller_ = balance_controller;
	stepmotor_control_  = stepmotor_control;
	IntoRobot.subscribe(BALANCE_CONTROL,NULL, paramsCallback);
	_joypad.begin(controlCallback);
}
void IntoBikeCloudInterface::controlCallback(void)
{
    if(_joypad.getUpkey() == 1)
       getInstance()->balance_controller_->setMoveDirection(1);
    else if(_joypad.getDownkey() == 1)
       getInstance()->balance_controller_->setMoveDirection(-1);
    else
       getInstance()->balance_controller_->setMoveDirection(0); 
       
    if(_joypad.getLeftkey() == 1)
       getInstance()->balance_controller_->setRotateDirection(1);
    else if(_joypad.getRightkey() == 1)
       getInstance()->balance_controller_->setRotateDirection(-1);
    else
       getInstance()->balance_controller_->setRotateDirection(0);   
}