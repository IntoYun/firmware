#include <Adafruit_NeoPixel/Adafruit_NeoPixel.h>

// 重要: 设置数据引脚、像素点个数、LED类型
#define PIXEL_COUNT 10
#define PIXEL_PIN D2
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// 滚轮效果
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

void colorAll(uint32_t c, uint8_t wait) 
{
    uint16_t i;

    for(i=0; i<strip.numPixels(); i++) 
    {
        strip.setPixelColor(i, c);
    }
    strip.show();
    delay(wait);
}

void colorWipe(uint32_t c, uint8_t wait) 
{
    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
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

// 彩虹效果
void rainbowCycle(uint8_t wait) 
{
    uint16_t i, j;

    for(j=0; j<256; j++) 
    { 
        for(i=0; i< strip.numPixels(); i++) 
        {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}


void setup() 
{
    strip.begin();
    strip.show();
}

void loop() 
{
    // 该例程展示如何显示，不要同时进行，否则将阻塞

    //strip.setPixelColor(0, strip.Color(255, 0, 255));
    //strip.show();

    //colorWipe(strip.Color(255, 0, 0), 50); // Red

    //colorWipe(strip.Color(0, 255, 0), 50); // Green

    //colorWipe(strip.Color(0, 0, 255), 50); // Blue

    rainbow(20);
}