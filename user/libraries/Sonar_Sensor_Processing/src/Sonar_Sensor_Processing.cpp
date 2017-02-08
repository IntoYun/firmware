/*
************************************************************************
* 作者:  IntoRobot Team 
* 版本:  V1.0.0
* 日期:  Dec. 21 2016
************************************************************************
IntoRobot-BCar: 超声测距数据处理
描述：
利用低通滤波滤除超声传感器测距噪声
*/
#include "Sonar_Sensor_Processing.h"
SonarSensorProcessing::SonarSensorProcessing(): rangefinder_(SONAR_TRIG, SONAR_ECHO, MIN_DISTANCE, MAX_DISTANCE)
{
}
void SonarSensorProcessing::begin(void)
{
	init();
}
void SonarSensorProcessing::init(void)
{
	obstacle_distance_ = 0.0f;
	rangefinder_.begin();
}
double SonarSensorProcessing::getDistance(void)
{
	obstacle_distance_ = rangefinder_.getDistanceCM();	// 得到距离（cm）
	obstacle_distance_ = lowpass_filter_.input(obstacle_distance_);
	return obstacle_distance_;
}
