
// Atom 和 HC_SRO4连接如下：
// Atom   HC-SR04
// GND     GND
// 5V      VCC
// D4      Trig
// D5      分压器输出，如下
//
//Echo --|
//       >
//       < 470欧姆电阻
//       >
//       --D5
//       >
//       < 470欧姆电阻
//       >
//GND ---|
//
// 默认的测距仪（rangefinder）测量距离是10cm到250cm. 超出这个范围显示为-1.
// 你可以改变这个距离范围，通过minCM和maxCM参数更改。例如：
// HC_SR04 rangefinder = HC_SR04(trigPin, echoPin, 5.0, 300.0);


#include <HC_SR04/HC_SR04.h>

double cm = 0.0;
double inches = 0.0;

int trigPin = D4;      // 发送管脚
int echoPin = D5;      // 接受管脚

HC_SR04 rangefinder = HC_SR04(trigPin, echoPin);

void setup() 
{
}

void loop() 
{
    cm = rangefinder.getDistanceCM();          // 得到距离（cm）
	SerialUSB.println(cm);
    inches = rangefinder.getDistanceInch();    // 得到距离（inch）
	SerialUSB.println(inches);
    delay(100);
}

