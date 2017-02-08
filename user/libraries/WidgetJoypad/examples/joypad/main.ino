/*该头文件引用由IntoRobot自动添加.*/
#include <WidgetJoypad_1/WidgetJoypad_1.h>

/*
************************************************************************
* 作者:  IntoRobot Team 
* 版本:  V1.0.0
* 日期:  03-30-15
************************************************************************
功能描述：
游戏控制手柄和遥杆

所需器件:

*/

WidgetJoypad joypad;

void SwitchCb(void)
{
    if(joypad.getUpkey())
    {
        digitalWrite(D7,HIGH);
    }
    else
    {
        digitalWrite(D7,LOW);
    }
}

void setup() 
{
    // put your setup code here, to run once.
    pinMode(D7,OUTPUT);
    joypad.begin(SwitchCb);
}

void loop() 
{
// put your main code here, to run repeatedly.
        //打印平台下发的手柄数据值
        SerialUSB.println(joypad.getUpkey());
        SerialUSB.println(joypad.getDownkey());
        SerialUSB.println(joypad.getLeftkey());
        SerialUSB.println(joypad.getRightkey());
        SerialUSB.println(joypad.getFnkey1());
        SerialUSB.println(joypad.getFnkey2());
        SerialUSB.println(joypad.getFnkey3());
        SerialUSB.println(joypad.getFnkey4());
        SerialUSB.println(joypad.getFnkey5());
        SerialUSB.println(joypad.getFnkey6());
        SerialUSB.println(joypad.getLeftJoystickX());
        SerialUSB.println(joypad.getLeftJoystickY());
        SerialUSB.println(joypad.getRightJoystickX());
        SerialUSB.println(joypad.getRightJoystickY());
        SerialUSB.println("xxxxxxxxxxxxxxxxxxxxxxxxxxx");
        
        delay(1000);
}