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
* 描述： IntoBike: 采用PID进行平衡控制
*/
#ifndef __INTOBIKE_BALANCE_CONTROL_H__
#define __INTOBIKE_BALANCE_CONTROL_H__
#include"application.h"
#define SPEED_HISTORY_NUM 20 
#define MAX_MAGNITUDE  5400
struct WheelsSpeed
{
	float left_;
	float right_;
}
;
struct BalanceControlParams
{
	float bc_kp_;
	float bc_kd_;
	float vc_kp_;
	float vc_ki_;
	float max_magnitude_;
}
;
struct DisplayDebugInfo
{
	float pitch_;
	float pitch_rate_;
	float control_l_;
	float control_r_;
	float velocity_pwm_;
	float balance_pwm_;
	float rotation_pwm_;
	float vc_iterm_;
	u8    filter_type_;
	WheelsSpeed wheels_command_speed_;
	BalanceControlParams control_params_;
}
;
class BalanceControl
{
	public:
	void begin();
	void begin(float sampling_time);
	void restart();
	DisplayDebugInfo getDebugInfo(u8 filter_type);
	void setParams(BalanceControlParams params);
	void setMoveDirection(s8 direction);
	void setRotateDirection(s8 direction);
	void setObstacleBlockedStatus(bool flag);
	WheelsSpeed doControl(float pitch_cur, float pitch_rate_cur,WheelsSpeed encoder);
	float velocityController(WheelsSpeed encoder);
	float balanceController(float pitch_cur, float pitch_rate_cur);
	float rotationController(WheelsSpeed encoder);
	float meanFilter(float left,float right);
	WheelsSpeed boundControlOutput(WheelsSpeed speed);
	WheelsSpeed linearizeControlOutput(WheelsSpeed speed);
	void remoteControl(char command);
	private:
	float dt_ = 0.01f;
	BalanceControlParams control_params_;
	float pitch_ref_       = 0.0f;
	float pitch_rate_ref_  = 0.0f;
	s8   move_direction_   = 0;
	s8   rotate_direction_ = 0;
	bool obstacle_blocked_  = false;
	uint16_t  blocked_backward_count_ = 0;
	uint16_t  blocked_turn_count_     = 0;
	float vc_iterm_        = 0.0f;
	float speed_buffer_[SPEED_HISTORY_NUM] =
	{
		0
	}
	;
	WheelsSpeed wheels_speed_;
	float velocity_        = 0.0f;
	float rotation_value_      = 0.0f;
	int   rotation_count_        = 0;
	float rotation_encoder_init_ = 0;
	DisplayDebugInfo debug_info_;
}
;
#endif