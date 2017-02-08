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
* 描述： IntoBike: state machine
*/
#ifndef INTOBIKE_STATE_MACHINE_H_
#define INTOBIKE_STATE_MACHINE_H_
#include "application.h"
enum IntoBikeState
{
	BLOCKED,
	IDLE,
	REMOTE_CONTROLED,
	STOP,
	FALL_DOWN,
	PICKED_UP
}
;
enum IntoBikeEvent
{
	BLOCKED_EVENT,
	UNBLOCKED_EVENT,
	REMOTE_CONTROLED_EVENT,
	OUTOFCONTROL_EVENT,
	FALL_DOWN_EVENT,
	STAND_UP_EVENT,
	PICKED_UP_EVENT,
	PUT_DOWN_EVENT
}
;
typedef struct EventInputData
{
	float pitch_;
	float pitch_rate_;
	float accel_z_;
	float obstacle_distance_;
	float encoder_left_;
	float encoder_right_;
}
;
typedef struct StateMachineInfo
{
	EventInputData data_;
	IntoBikeEvent event_;
	IntoBikeState state_;
}
;
class IntoBikeStateMachine
{
	public:
	void begin(void);
	void update(IntoBikeEvent event);
	IntoBikeState getCurrentState();
	bool detectFallDown(float angle, float angle_rate);
	bool detectObstacle(float distance);
	bool detectPutDown(float accel,float angle);
	bool detectStandUp(float angle_rate,float angle);
	bool detectPickUp(float accel,float angle,int encoder_left,int encoder_right);
	StateMachineInfo getInfo(void);
	IntoBikeEvent detectEvent(EventInputData input);
	private:
	IntoBikeState current_state_;
	EventInputData data_;
	IntoBikeEvent current_event_;
	u16 pickup_flag_ = 0;
	u16 pickup_count0_ = 0;
	u16 pickup_count1_ = 0;
	u16 pickup_count2_ = 0;
	u16 pickup_count3_ = 0;
	u16 putdown_count_ = 0;
	u16 putdown_flag_  = 0;
	u16 standup_flag_  = 0;
	u16 standup_count_ = 0;
}
;
#endif