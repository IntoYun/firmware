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

#ifndef __INTOBIKE_STEPMOTOR_CONTROL_H__
#define __INTOBIKE_STEPMOTOR_CONTROL_H__

#include "application.h"

#define MAX_SPEED 13000
#define MIN_SPEED 15

class StepMotorControl
{
public:
	void  begin(int fr_pin, int step_pin, int stop_pin);
	void  init(void);
	void  disableMotors();
	void  enableMotors();
	void  setMaxSpeed(int speed);
	void  setMinSpeed(int speed);
    void  setMotorSpeed(int32_t speed);
    bool  isStopped(void);
    int32_t getEncoder(void);
private:
    int32_t motor_speed_;
    int  fr_pin_;
    int  step_pin_;
    int  stop_pin_;
    int32_t max_speed_;
    int32_t min_speed_;
    bool is_stopped_;
}
;
#endif