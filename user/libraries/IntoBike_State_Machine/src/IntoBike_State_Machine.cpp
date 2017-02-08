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
#include "IntoBike_State_Machine.h"
void IntoBikeStateMachine::begin(void)
{
	current_state_ = IDLE;
	current_event_ = STAND_UP_EVENT;
	u16 pickup_flag_ = 0;
	u16 pickup_count0_ = 0;
	u16 pickup_count1_ = 0;
	u16 pickup_count2_ = 0;
	u16 putdown_count_ = 0;
	u16 putdown_flag_  = 0;
	u16 standup_flag_  = 0;
	u16 standup_count_ = 0;
}
void IntoBikeStateMachine::update(IntoBikeEvent event)
{
	switch(current_state_)
	{
		case BLOCKED:
		{
			if(event == UNBLOCKED_EVENT)  current_state_ = IDLE;
			else if(event == PICKED_UP_EVENT) current_state_ = PICKED_UP;
			else if(event == FALL_DOWN_EVENT) current_state_ = FALL_DOWN;
			break;
		}
		case FALL_DOWN:
		{
			if(event == STAND_UP_EVENT)
			current_state_ = IDLE;
			break;
		}
		case PICKED_UP:
		{
			if(event == PUT_DOWN_EVENT)
			current_state_ = IDLE;
			else if(event == FALL_DOWN_EVENT) 
			current_state_ = FALL_DOWN;
			break;
		}
		case REMOTE_CONTROLED:
		{
			if(event == PICKED_UP_EVENT)
			current_state_ = PICKED_UP;
			else if(event == FALL_DOWN_EVENT)
			current_state_ = FALL_DOWN;
			else if(event == BLOCKED_EVENT)
			current_state_ = BLOCKED;
			else if(event == OUTOFCONTROL_EVENT)
			current_state_ = IDLE;
			break;
		}
		case IDLE:
		{
			if(event == PICKED_UP_EVENT)
			current_state_ = PICKED_UP;
			else if(event == FALL_DOWN_EVENT)
			current_state_ = FALL_DOWN;
			else if(event == BLOCKED_EVENT)
			current_state_ = BLOCKED;
			else if(event == REMOTE_CONTROLED_EVENT)
			current_state_ = REMOTE_CONTROLED;
			break;
		}
	}
}
IntoBikeState IntoBikeStateMachine::getCurrentState()
{
	return current_state_;
}
bool IntoBikeStateMachine::detectFallDown(float angle, float angle_rate)
{
	if(abs(angle) > 70.0 && abs(angle_rate) < 10)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
bool IntoBikeStateMachine::detectObstacle(float distance)
{
	if(distance < 3.0)
	return 0;	//error
	else if(distance < 40)
	return 1;
	else
	return 0;
}
bool IntoBikeStateMachine::detectPutDown(float accel,float angle)
{
	if(current_state_ == PICKED_UP)
	{
		if(accel < -0.8 && (abs(angle) < 20))
		{
			return 1;
		}
	}
	return 0;
}
bool IntoBikeStateMachine::detectStandUp(float angle_rate,float angle)
{
	if(current_state_ == FALL_DOWN)
	{
		if(abs(angle) < 10)
		{
			return 1;
		}
	}
	return 0;
}
bool IntoBikeStateMachine::detectPickUp(float accel,float angle,int encoder_left,int encoder_right)
{
	if(pickup_flag_ == 0)
	{
		if(abs(encoder_left) + abs(encoder_right) < 700)                    
		pickup_count0_++;
		else 
		pickup_count0_ = 0;
		if(pickup_count0_ > 10)
		{
			pickup_flag_ = 1;
			pickup_count0_ = 0;
		}
	}
	if(pickup_flag_ == 1)
	{
		if(++pickup_count1_ > 400)
		{
			pickup_count1_ = 0;
			pickup_flag_ = 0;
		}
		if(accel > 1.4 && (abs(angle) < 10))
		{
			pickup_flag_ = 2;
		}
	}
	if(pickup_flag_ == 2)
	{
		if(++pickup_count2_ > 400)
		{
			pickup_count2_ = 0;
			pickup_flag_ = 0;
		}
		if(abs(encoder_left) + abs(encoder_right) > 9900)
		{
			pickup_flag_ = 0;
			pickup_count0_ = 0;
			pickup_count1_ = 0;
			pickup_count2_ = 0;
			return 1;
		}
	}
	return 0;
}
IntoBikeEvent IntoBikeStateMachine::detectEvent(EventInputData input)
{
	data_ = input;
	if(detectPutDown(input.accel_z_,input.pitch_))
	{
		current_event_ = PUT_DOWN_EVENT;
	}
	else if(detectStandUp(input.pitch_rate_, input.pitch_))
	{
		current_event_ = STAND_UP_EVENT;
	}
	else if(detectPickUp(input.accel_z_,input.pitch_, input.encoder_left_, input.encoder_right_))
	{
		current_event_ = PICKED_UP_EVENT;
	}
	else if(detectFallDown(input.pitch_, input.pitch_rate_))
	{
		current_event_ = FALL_DOWN_EVENT;
	}
	else if(detectObstacle(input.obstacle_distance_))
	{
		current_event_ = BLOCKED_EVENT;
	}
	else
	{
		if(current_state_ != PICKED_UP && current_state_ != FALL_DOWN)
		current_event_ = UNBLOCKED_EVENT;
	}
	update(current_event_);
	return current_event_;
}
StateMachineInfo IntoBikeStateMachine::getInfo(void)
{
	StateMachineInfo info;
	info.data_  = data_;
	info.state_ = current_state_;
	info.event_ = current_event_;
	return info;
}