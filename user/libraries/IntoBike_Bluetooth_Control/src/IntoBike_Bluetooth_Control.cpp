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
由手机或者电脑，经由蓝牙给IntoBike发送控制指令
蓝牙接受指令的列表：
d - oled屏幕显示数据的切换
l - 小车左转
r - 小车右转
f - 小车前进
b - 小车后退
s - 小车停止
*/
#include "IntoBike_Bluetooth_Control.h"
char bluetooth_buffer_[BLUETOOTH_BUFFER_SIZE];

bool bluetoothDataReady(bool flag)
{
	int buffer_index = 0;
	char inByte;
	char end_char[2];
	if(flag)
	{
		while(Serial.available())
		{
			bluetooth_buffer_[0] = Serial.read();
			return true;
		}
		return false;
	}
	else
	{
		while (Serial.available())
		{
			if(Serial.read() == 'S' && Serial.peek() == 'T')
			{
				Serial.read();
				while(Serial.available())
				{
					inByte = Serial.read();
					if(inByte == 'E' && Serial.peek() == 'N')
					{
						Serial.read();
						bluetooth_buffer_[buffer_index] = 0;
						return true;
					}
					else
					{
						bluetooth_buffer_[buffer_index] = inByte;
						buffer_index++;
					}
					if(buffer_index >= BLUETOOTH_BUFFER_SIZE)
					{
						return false;
					}
				}
			}
		}
		return false;
	}
}
bool processBluetoothData(char *command, bool flag)
{
	if(flag)
	{   //s-stop, r-right-turn, f-forward, l-left-trun, b-backward
		*command = bluetooth_buffer_[0];
		return true;
	}
	else
	{
		aJsonClass aJson;
		SerialUSB.printf("Str:%s\n", bluetooth_buffer_);
		aJsonObject *root = aJson.parse((char *)bluetooth_buffer_);
		if(root == NULL)
		{
			aJson.deleteItem(root);
			return false;
		}
		aJsonObject *data_X = aJson.getObjectItem(root, "X");
		if(data_X == NULL)
		{
			aJson.deleteItem(root);
			return false;
		}
		aJsonObject *data_Y = aJson.getObjectItem(root, "Y");
		if(data_Y == NULL)
		{
			aJson.deleteItem(root);
			return false;
		}
		int data_X_int = data_X->valueint;
		int data_Y_int = data_Y->valueint;
		aJson.deleteItem(root);
		float angle = atan2(data_Y_int, data_X_int) / 3.1415926 * 180;
		if(data_Y_int == 0 && data_X_int == 0)
		*command = 0;		//stop
		else if(abs(angle) < 20)
		*command = 1;		//right turn
		else if(abs(angle - 90)  < 20)
		*command = 2;		//forward
		else if(abs(angle - 180) < 20 || abs(angle + 180) < 20)
		*command = 3;		//left turn
		else if(abs(angle + 90) < 20)
		*command = 4;		//backward
		SerialUSB.printf("Angle:%f\n", angle);
		SerialUSB.printf("Command:%d\n", command);
		SerialUSB.printf("dataX:%d\n", data_X_int);
		SerialUSB.printf("dataY:%d\n", data_Y_int);
		return true;
	}
}