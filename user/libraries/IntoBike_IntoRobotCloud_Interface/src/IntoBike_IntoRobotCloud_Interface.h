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

#ifndef __INTOBIKE_INTOROBOTCLOUD_INTERFACE_H__
#define __INTOBIKE_INTOROBOTCLOUD_INTERFACE_H__

/*该头文件引用由IntoRobot自动添加.*/
#include "IntoBike_StepMotor_Control.h"
/*该头文件引用由IntoRobot自动添加.*/
#include "WidgetJoypad.h"
/*该头文件引用由IntoRobot自动添加.*/
#include "IntoBike_Balance_Control.h"

#include"application.h"
#define BALANCE_CONTROL "channel/balancePid_0/cmd/control"
#define CONTROL_COMMAND "channel/joypad_0/cmd/control"

#ifdef __cplusplus
extern "C"
{
#endif 
void paramsCallback(uint8_t * payload, uint32_t len);
void controlCallback(uint8_t * payload, uint32_t len);
#ifdef __cplusplus
}
#endif 
class IntoBikeCloudInterface
{
	public:
	static IntoBikeCloudInterface* getInstance()
	{
		return const_cast<IntoBikeCloudInterface *>(IntoBikeCloudInterface::m_singleton);
	}
	void begin(BalanceControl *balance_controller, StepMotorControl stepmotor_control[]);
	void begin(StepMotorControl stepmotor_control[], BalanceControl *balance_controller);
	static void controlCallback(void);
	static void destroyInstance()
	{
		if(IntoBikeCloudInterface::m_singleton != NULL)
		{
			delete IntoBikeCloudInterface::m_singleton;
			IntoBikeCloudInterface::m_singleton = NULL;
		}
	}
	BalanceControl *balance_controller_;
	StepMotorControl *stepmotor_control_;

	private :
	static IntoBikeCloudInterface *m_singleton;
}
;
#endif