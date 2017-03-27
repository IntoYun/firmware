#include <Adafruit_NeoPixel/Adafruit_NeoPixel.h>

// 重要: 设置数据引脚、像素点个数、LED类型
#define PIXEL_PIN D2
#define PIXEL_COUNT 10
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
void rainbow(uint8_t wait);
uint32_t Wheel(byte WheelPos); 

void setup() 
{
    strip.begin();//初始化
    strip.show(); 
}

void loop() 
{
    rainbow(20);
}

void rainbow(uint8_t wait) 
{
    uint16_t i, j;

    for(j=0; j<256; j++) 
    {
        for(i=0; i<strip.numPixels(); i++) 
        {
            strip.setPixelColor(i, Wheel((i+j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

//滚轮效果
uint32_t Wheel(byte WheelPos) 
{
    if(WheelPos < 85) 
    {
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    } 
    else if(WheelPos < 170) 
    {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } 
    else 
    {
        WheelPos -= 170;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}

