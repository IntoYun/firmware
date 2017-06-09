
//SYSTEM_MODE(MANUAL);
SerialDebugOutput debugOutput(115200, ALL_LEVEL);

/*
 ************************************************************************
 * 作者:  IntoRobot Team
 * 版本:  V1.0.0
 * 日期:  03-30-15
 ************************************************************************
 功能描述：
 测距仪：测量障碍物距离

 所需器件:
 1.HC-SR04超声传感器模块

 接线说明:
 HC-SR04                      核心板
 1.VCC                        +5V
 2.GND                        GND
 3.Trig                        D0
 4.Echo                        D1

 说明：带有“D”的为数字管脚，带有“A”的为模拟管脚，接线时请确认核心板引脚，避免接线错误。
 */

#include "HC_SR04.h"

// 超声测距仪应用topic
#define DIASTIMETER_DATA_DISTANCE   "channel/diastimeter_0/data/distance"

double cm = 0.0;

#define TRIPIN  D0     // 发送管脚
#define ECHOPIN D1     // 接收管脚

HC_SR04 rangefinder = HC_SR04(TRIPIN, ECHOPIN);

void setup()
{
    rangefinder.begin(); // 配置管脚模式
}

void loop()
{
    cm = rangefinder.getDistanceCM(); // 得到距离（cm）
    if(cm != -1)
    {
        // 把测量得到的距离数据发送到云平台
        IntoRobot.publish(DIASTIMETER_DATA_DISTANCE, cm);
    }

    delay(2000);
}
