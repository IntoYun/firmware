/*该头文件引用由IntoRobot自动添加.*/
#include "MeanFilter.h"
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
IntoBike: 平衡小车的姿态角及角速度计算。
描述：通过Kalman滤波器融合加速度计和陀螺仪的信息
系统的状态方程为：
X(k+1) = A * X(k) + B * u + w1
Y(k)   = C * X(k) + w2
其中，
X为系统状态，为向量（pitch, pitch rate offset);
Y为测量量，即小车倾角；
u为系统输入，为上一时刻的倾角速度（pitch rate）；
w1为过程噪声，方差为Q；w2为观测噪声，方差为R；
A为状态转移矩阵，B为控制矩阵，C为观测矩阵
*/
#ifndef __INTOBIKE_ACCELGYRO_SENSOR_PROCESSING_H__
#define __INTOBIKE_ACCELGYRO_SENSOR_PROCESSING_H__
#include"application.h"    //used for sensor library include
/*该头文件引用由IntoRobot自动添加.*/
#include "Matrix.h"
/*--------------- Macros -----------------*/
#define PI 3.14159265358
#define ACC_FILTER_COUNT 16
#define ACCEL_SENSOR_RANGE BMI160_ACCEL_RANGE_4G
#define GYRO_SENSOR_RANGE  BMI160_GYRO_RANGE_2000_DEG_SEC
using namespace mm;
struct PoseData
{
	float pitch_;
	float pitch_rate_;
}
;
class AccelGyroSensorProcessing
{
	public:
	void begin();
	void begin(float sampling_time, u8 filter_type);
	void init();
	PoseData getRawData(void);
	void doMeasurement();
	void processData(void);
	bool detectPickUp();
	int  event(void);
	PoseData getFilteredData(void);
	u8   getFilterType(void);
	float    getAccel(u8 axis);
	Vector2f kalmanFilter(float angle, float angle_rate);
	Vector2f complementaryFilter(float value_pre, float angle_accel, float gyro);
	void     accFilter();	//low-pass filter
	private:
	float accel_raw_[3];
	float gyro_raw_[3];
	float accel_meanfiltered_[3];
	float accel_raw_buf_x_[ACC_FILTER_COUNT]=
	{
		0
	}
	;
	float accel_raw_buf_y_[ACC_FILTER_COUNT]=
	{
		0
	}
	;
	float accel_raw_buf_z_[ACC_FILTER_COUNT]=
	{
		0
	}
	;
	float dt_;	//sampling time
	u8    filter_type_ = 0;	//Kalman Filter
	Matrix<float,2,2> Q_, A_;
	Matrix<float,2,1> B_;
	Matrix<float,1,2> C_;
	Matrix<float,2,1> state_k_;
	Matrix<float,2,2> P_k_;
	Matrix<float,2,2> I_;
	Matrix<float, 2, 1> K_k_;	//Kalman gain
	int display_ct_;	//counter for debug purpose
	float R_;
	float measurement_k_;
	float input_;
	PoseData pose_data_;

	void kalman_filter(float angle_m, float gyro_m, float *angle_f, float *angle_dot_f);
	float angle_, angle_dot_;
	const float Q_angle_ = 0.002, Q_gyro_ = 0.002, R_angle_ = 0.5;
	float P_[2][2]=
	{
		{
			1, 0
		}
		,
		{
			0, 1
		}
	}
	;
	float Pdot_[4] =
	{
		0, 0, 0, 0
	}
	;
	const u8 C_0_ = 1;
	float q_bias_, angle_err_, PCt_0_, PCt_1_, E_, K_0_, K_1_, t_0_, t_1_;
}
;
#endif