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
#ifndef INTOBIKE_BLUETOOTH_CONTROL_H_
#define INTOBIKE_BLUETOOTH_CONTROL_H_
#include "application.h"
#define BLUETOOTH_BUFFER_SIZE 100
bool bluetoothDataReady(bool flag);
bool processBluetoothData(char * command, bool flag);
extern char bluetooth_buffer_[BLUETOOTH_BUFFER_SIZE];
#endif