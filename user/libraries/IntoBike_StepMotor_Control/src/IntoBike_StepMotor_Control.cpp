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
* 描述：
IntoBike: 步进电机控制接口
步进电机为1.8度分辨率，驱动芯片LV8731V选择16细分，即步进电机的分辨率为0.1125度。
经过实验测试，电机的最高响应频率约为20000步／秒，即每秒钟电机最大转动6.25圈。
为了保证精度，我们将最大转速限定为13000步／秒，即4.06圈／秒，
因此我们限定步进电机驱动芯片LV8731V的最大输入PWM脉冲频率为13KHz
*/
#include "IntoBike_StepMotor_Control.h"
void StepMotorControl::begin(int fr_pin, int step_pin, int stop_pin)
{
	fr_pin_    = fr_pin;
	step_pin_  = step_pin;
	stop_pin_  = stop_pin;
	init();
}
void StepMotorControl::init(void)
{
	pinMode(fr_pin_,   OUTPUT);
	pinMode(step_pin_, OUTPUT);
	pinMode(stop_pin_, OUTPUT);
	digitalWrite(fr_pin_,   LOW);
	digitalWrite(step_pin_, LOW);
	digitalWrite(stop_pin_, LOW);
	motor_speed_ = 0;
	is_stopped_   = false;
	max_speed_ = MAX_SPEED;
	min_speed_ = MIN_SPEED;
}
void StepMotorControl::setMotorSpeed(int32_t speed)
{
	uint32_t abs_speed;
	motor_speed_ = speed;
	abs_speed = abs(motor_speed_);

	if(abs_speed < min_speed_)  abs_speed = min_speed_;
	if(abs_speed > max_speed_)  abs_speed = max_speed_;

	if(motor_speed_ >= 0)
	{
		digitalWrite(fr_pin_,  HIGH);
		analogWrite(step_pin_, 200, abs_speed);
	}
	else
	{
		digitalWrite(fr_pin_,  LOW);
		analogWrite(step_pin_, 200, abs_speed);
	}
}
int32_t  StepMotorControl::getEncoder(void)
{
	return motor_speed_;
}
void StepMotorControl::enableMotors()
{
	digitalWrite(stop_pin_, HIGH);
	is_stopped_ = false;
}
void StepMotorControl::disableMotors()
{
	digitalWrite(stop_pin_, LOW);
	is_stopped_ = true;
	motor_speed_ = 0;
}
bool StepMotorControl::isStopped(void)
{
	return is_stopped_;
}
void StepMotorControl::setMaxSpeed(int speed)
{
	max_speed_ = speed;
}
void StepMotorControl::setMinSpeed(int speed)
{
	min_speed_ = speed;
}