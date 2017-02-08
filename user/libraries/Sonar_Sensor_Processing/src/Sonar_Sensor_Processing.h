#ifndef __SONAR_SENSOR_PROCESSING_H__
#define __SONAR_SENSOR_PROCESSING_H__

/*该头文件引用由IntoRobot自动添加.*/
#include "HC_SR04.h"
/*该头文件引用由IntoRobot自动添加.*/
#include "MeanFilter.h"
/*
************************************************************************
* 作者:  IntoRobot Team 
* 版本:  V1.0.0
* 日期:  Dec. 21 2016
************************************************************************
IntoRobot-BCar: 超声测距数据处理
描述：
利用低通滤波滤除超声传感器测距噪声
Neutron 和 HC_SRO4连接如下：
GND     GND
3.3V    VCC
A1      Trig
A0      echo
*/
#define SONAR_TRIG A1
#define SONAR_ECHO A0
#define MIN_DISTANCE 5.0
#define MAX_DISTANCE 500.0  //IMPORTANT

class SonarSensorProcessing
{
public:
    SonarSensorProcessing();
	void  begin();
	void  init();
	double lowPassFilter();
	double getDistance(void);
private:
    double obstacle_distance_;
    HC_SR04 rangefinder_;
    MeanFilter lowpass_filter_;
}
;
#endif