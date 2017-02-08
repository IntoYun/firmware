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
#include "IntoBike_AccelGyro_Sensor_Processing.h"
void AccelGyroSensorProcessing::begin(void)
{
	dt_ = 0.01;	//default sampling time
	init();
}
void AccelGyroSensorProcessing::begin(float sampling_time, u8 filter_type)
{
	dt_ = sampling_time;	//default sampling time
	filter_type_ = filter_type;
	init();
}
void AccelGyroSensorProcessing::init(void)
{
	BMI160.begin();	//BMI160(BMI150) Accelerometer 6-axis gyro sensor
	BMI160.setRange(ACCEL_SENSOR_RANGE, GYRO_SENSOR_RANGE);
	A_(0,0) = 1.0;	A_(0,1) = - dt_;
	A_(1,0) = 0;	A_(1,1) = 1.0;
	B_(0,0) = dt_;
	B_(1,0) = 0;
	C_(0,0) = 1;	C_(0,1) = 0;
	Q_(0,0) = 0.002;	Q_(0,1) = 0;
	Q_(1,0) = 0;	Q_(1,1) = 0.002;
	R_ = 0.5;
	I_(0,0) = 1;	I_(0,1) = 0;
	I_(1,0) = 0;	I_(1,1) = 1;
	P_k_(0,0) = 1.0f;	P_k_(0,1) = 0.0f;
	P_k_(1,0) = 0.0f;	P_k_(1,1) = 1.0f;
	measurement_k_ = 0.0f;
	input_         = 0.0f;
	state_k_(0, 0) = 0.0f;
	state_k_(1, 0) = 0.0f;
	processData();
}
u8 AccelGyroSensorProcessing::getFilterType()
{
    return filter_type_;
}
//NOTE: Run processData before calling this function
PoseData AccelGyroSensorProcessing::getFilteredData(void)
{
	return pose_data_;
}
void AccelGyroSensorProcessing::doMeasurement(void)
{
	int16_t accel[3] =
	{
		0
	}
	;
	int16_t gyro[3]  =
	{
		0
	}
	;
	BMI160.getAccelGyroData(accel, gyro);
	accel_raw_[0] = - accel[0] * 0.000122;	//accel / 8192 for -4G to 4G range, unit: G
	accel_raw_[1] = - accel[1] * 0.000122;	//accel / 8192 for -4G to 4G range, unit: G
	accel_raw_[2] = - accel[2] * 0.000122;	//accel / 8192 for -4G to 4G range, unit: G
	gyro_raw_[0]  = gyro[0] * 0.06097561;	//gyro / 16.4
	gyro_raw_[1]  = gyro[1] * 0.06097561;	//gyro / 16.4
	gyro_raw_[2]  = gyro[2] * 0.06097561;	//gyro / 16.4
	accFilter();
}
void AccelGyroSensorProcessing::processData(void)
{
	Matrix<float, 2, 1> value_filtered;
	float pitch, pitch_rate;
	float angle_f, angle_dot_f;
	doMeasurement();
	pitch      = atan2(accel_meanfiltered_[0], sqrt(accel_meanfiltered_[2] * accel_meanfiltered_[2] + accel_meanfiltered_[1] * accel_meanfiltered_[1])) / PI *180.0;
	pitch_rate = gyro_raw_[1];
	pitch     -= 1.5; // bias
	pitch_rate = gyro_raw_[1] - 0.06; //  bias
	switch(filter_type_)
	{
		case 0:
		//value_filtered = complementaryFilter(pose_data_.pitch_, pitch, pitch_rate);
		kalman_filter(pitch, pitch_rate, &angle_f, &angle_dot_f);
		value_filtered(0,0) = angle_f;
		value_filtered(1,0) = angle_dot_f;
		//value_filtered = kalmanFilter(pitch, pitch_rate);
		break;
		case 1:
		value_filtered = complementaryFilter(pose_data_.pitch_, pitch, pitch_rate);
		break;
		default:
		break;
	} 
	pose_data_.pitch_       = value_filtered(0,0);  
	//pose_data_.pitch_rate_  = pitch_rate;
	pose_data_.pitch_rate_  = value_filtered(1,0);  
}
//get raw accel and gyro data
PoseData AccelGyroSensorProcessing::getRawData(void)
{
	doMeasurement();
	PoseData temp;
	temp.pitch_          = atan2(accel_raw_[0], accel_raw_[2]) / PI *180.0;
	temp.pitch_rate_     = gyro_raw_[1] / PI * 180.0;
	return temp;
}
//get raw accel at axis
float AccelGyroSensorProcessing::getAccel(u8 axis)
{
	doMeasurement();
	if(axis == 0)
	{
		return accel_raw_[0];
	}
	else if(axis == 1)
	{
		return accel_raw_[1];
	}
	else
	{
		return accel_raw_[2];
	}
}
/*****************************************************
* Function Name  :  Complementary_filter(float value_pre, float angle_accel, float gyro)
* Description    :  Complementary values from both accel and gyro sensors
* Inputs         :  None
* Outputs        :  None
* Notes          :  
******************************************************/
Vector2f AccelGyroSensorProcessing::complementaryFilter(float value_pre, float angle_accel, float gyro)
{
	float temp;
	Vector2f result;
	temp = 0.98 * (value_pre - dt_ * gyro) + 0.02 * angle_accel;
	result(0,0) = temp;
	result(1,0) = gyro;
	return result;
}
/*****************************************************
* Function Name  :  kalmanFilter(float Accel,float Gyro)
* Description    :  Filter values from both accel and gyro sensors
* Inputs         :  None
* Outputs        :  None
* Notes          :  
******************************************************/
Vector2f AccelGyroSensorProcessing::kalmanFilter(float angle, float angle_rate)
{
	Vector2f result;
	float inv;
	//Prediction
	state_k_(0,0) = A_(0,0) * state_k_(0,0) + A_(0,1) * state_k_(1,0) - B_(0,0) * input_;
	state_k_(1,0) = A_(1,0) * state_k_(0,0) + A_(1,1) * state_k_(1,0) + B_(1,0) * input_;
	P_k_ = A_ * P_k_ * A_.Transpose() + Q_;
	//Correction
	measurement_k_ = angle;
	inv = 1.0 / (P_k_(0,0) + R_);
	K_k_ = P_k_ * C_.Transpose() * inv;
	state_k_ = state_k_ + K_k_ * (measurement_k_ - state_k_(0,0));
	P_k_ = (I_ - K_k_ * C_) * P_k_;
	result(0,0) = state_k_.get(0,0);
	input_      = angle_rate - state_k_.get(1,0);
	result(1,0) = input_;
	return result;
}
void AccelGyroSensorProcessing::kalman_filter(float angle_m, float gyro_m, float *angle_f, float *angle_dot_f)			
{
  angle_ += (gyro_m - q_bias_) * dt_;
	
  Pdot_[0] = Q_angle_ - P_[0][1] - P_[1][0];
  Pdot_[1] = -P_[1][1];
  Pdot_[2] = -P_[1][1];
  Pdot_[3] = Q_gyro_;
	
  P_[0][0] += Pdot_[0] * dt_;
  P_[0][1] += Pdot_[1] * dt_;
  P_[1][0] += Pdot_[2] * dt_;
  P_[1][1] += Pdot_[3] * dt_;
	
  angle_err_ = angle_m - angle_;
	
  PCt_0_=C_0_ * P_[0][0];
  PCt_1_=C_0_ * P_[1][0];
	
  E_ = R_angle_ + C_0_ * PCt_0_;
	
  K_0_ = PCt_0_ / E_;
  K_1_ = PCt_1_ / E_;
	
  t_0_ = PCt_0_;
  t_1_ = C_0_ * P_[0][1];

  P_[0][0] -= K_0_ * t_0_;
  P_[0][1] -= K_0_ * t_1_;
  P_[1][0] -= K_1_ * t_0_;
  P_[1][1] -= K_1_ * t_1_;
		
  angle_  += K_0_ * angle_err_;
  q_bias_ += K_1_ * angle_err_;
  angle_dot_ = gyro_m - q_bias_;

  *angle_f = angle_;
  *angle_dot_f = angle_dot_;
}
///***********************************************************************/

void AccelGyroSensorProcessing::accFilter(void)
{
	int i;
	float sum_x(0.0f), sum_y(0.0f), sum_z(0.0f);
	for (i = 1 ; i < ACC_FILTER_COUNT; i++)
	{
		accel_raw_buf_x_[i - 1] = accel_raw_buf_x_[i];
		accel_raw_buf_y_[i - 1] = accel_raw_buf_y_[i];
		accel_raw_buf_z_[i - 1] = accel_raw_buf_z_[i];
	}
	accel_raw_buf_x_[ACC_FILTER_COUNT - 1] = accel_raw_[0];
	accel_raw_buf_y_[ACC_FILTER_COUNT - 1] = accel_raw_[1];
	accel_raw_buf_z_[ACC_FILTER_COUNT - 1] = accel_raw_[2];
	for (i = 0 ; i < ACC_FILTER_COUNT; i++)
	{
		sum_x += accel_raw_buf_x_[i];
		sum_y += accel_raw_buf_y_[i];
		sum_z += accel_raw_buf_z_[i];
	}
	accel_meanfiltered_[0] = sum_x / ACC_FILTER_COUNT;
	accel_meanfiltered_[1] = sum_y / ACC_FILTER_COUNT;
	accel_meanfiltered_[2] = sum_z / ACC_FILTER_COUNT;
}