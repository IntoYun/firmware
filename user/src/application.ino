
Serial1DebugOutput debugOutput(115200, ALL_LEVEL);

#include "application.h"

#define LED_PIN  36
uint8_t count;
void countISR(void)
{
    count++;
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(A5,INPUT_PULLUP);
    attachInterrupt(A5,countISR,FALLING);
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);              // wait for a second
    digitalWrite(LED_PIN, HIGH);    // turn the LED off by making the voltage LOW
    delay(900);              // wait for a second
    DEBUG("count = %d",count);
}


#if 0
//舵机
Servo myservo;
int pos = 0;
void setup()
{
    myservo.attach(A5);  // attaches the servo on pin 9 to the servo object 
}

void loop()
{
    for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
        myservo.write(pos);              // tell servo to go to position in variable 'pos' 
        delay(15);                       // waits 15ms for the servo to reach the position 
    }
    for(pos = 180; pos>=0; pos -= 1)     // goes from 180 degrees to 0 degrees 
    {
        myservo.write(pos);              // tell servo to go to position in variable 'pos' 
        delay(15);                       // waits 15ms for the servo to reach the position 
    }
}
#endif

#if 0
void setup()
{
    pinMode(LED_PIN, OUTPUT);
#if 0
   //pwm
    pinMode(D0,OUTPUT);
    pinMode(D1,OUTPUT);
    pinMode(D2,OUTPUT);
    pinMode(D4,OUTPUT);
    pinMode(D5,OUTPUT);
    pinMode(A0,OUTPUT);
    pinMode(A4,OUTPUT);
    pinMode(A5,OUTPUT);
    tone(D4,1000,1000000);
    analogWrite(D0,10);
    analogWrite(D1,20);
    analogWrite(D2,50);
    analogWrite(D4,90);
    analogWrite(D5,100);
    analogWrite(A0,150);
    analogWrite(A4,180);
    analogWrite(A5,220);
   #endif
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);              // wait for a second
    digitalWrite(LED_PIN, HIGH);    // turn the LED off by making the voltage LOW
    delay(900);              // wait for a second
}
#endif
//====================================================


#if 0
/*
 * Atom 默认程序
 */
#include "application.h"

#define SMARTLIGHT_CMD_SWITCH    "channel/smartLight_0/cmd/switch"   //开关命令
#define SMARTLIGHT_DATA_STATUS   "channel/smartLight_0/data/status"  //开关状态

#define LEDPIN    D7    //定义灯泡控制引脚

void smartLightSwitchCb(uint8_t *payload, uint32_t len)
{
    if(payload[0] == '1')
    {
        digitalWrite(LEDPIN, HIGH);     // 打开灯泡
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"1");
    }
    else
    {
        digitalWrite(LEDPIN, LOW);      //关闭灯泡
        IntoRobot.publish(SMARTLIGHT_DATA_STATUS,"0");
    }
}

void setup()
{
    //初始化
    pinMode(LEDPIN, OUTPUT);
    //接收灯开关命令
    IntoRobot.subscribe(SMARTLIGHT_CMD_SWITCH, NULL, smartLightSwitchCb);
}

void loop()
{
}
#endif

#if 0
#define LED_PIN D7

void setup()
{
    //Serial.begin(115200);
    //set_logger_output(log_output, ALL_LEVEL);
    //char version[32];
    // initialize digital pin 13 as an output.
    pinMode(LED_PIN, OUTPUT);
    pinMode(A0, OUTPUT);
    DEBUG("GPIO Test\r\n");
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
    DEBUG("digital Read: %d\r\n", digitalRead(A0));
}
#endif

#if 0
#define testA A0
#define LED_PIN D7

void setup()
{
   pinMode(testA, INPUT);
   pinMode(LED_PIN, OUTPUT);
   DEBUG("AnalogRead Test\r\n");
}

// the loop function runs over and over again forever
void loop()
{
    uint16_t value = analogRead(testA);
    DEBUG("analogRead value: %d", value);
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second

}
#endif

#if 0
#include "gy30.h"

#define LED_PIN D7

GY30 gy30;
void setup()
{
    DEBUG("I2C Test\r\n");
    pinMode(LED_PIN, OUTPUT);
    gy30.begin();
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                   // wait for a second
    DEBUG("GY30 value: %f", gy30.Read());
    //DEBUG("Runing !");
}
#endif

#if 0
#define LED_PIN D7
#define testPinPWM A0
#define INTERRUPT_PIN D0

uint8_t flag = 0;
void Blink(void)
{
	if(flag == 0)
	{
		flag = 1;
		digitalWrite(LED_PIN,HIGH);
	}
	else
	{
		flag = 0;
		digitalWrite(LED_PIN,LOW);
	}
}

void setup()
{
    DEBUG("Interrupt Test\r\n");
    pinMode(LED_PIN, OUTPUT);

	pinMode(INTERRUPT_PIN, INPUT_PULLUP);
	pinMode(testPinPWM, OUTPUT);
	attachInterrupt(INTERRUPT_PIN, Blink, FALLING);
    DEBUG("After setup\r\n");
}

// the loop function runs over and over again forever
void loop()
{
    DEBUG("Enter loop\r\n");
    attachInterrupt(INTERRUPT_PIN, Blink, FALLING);

	for(uint8_t i = 0; i < 5; i++)
	{
		digitalWrite(testPinPWM, HIGH);
		delay(500);
		digitalWrite(testPinPWM, LOW);
		delay(500);
	}

	detachInterrupt(INTERRUPT_PIN);
	delay(1000);
	attachInterrupt(INTERRUPT_PIN,Blink, FALLING);

	for(uint8_t i = 0; i < 5; i++) // ledÉÁË¸
	{
		digitalWrite(testPinPWM, HIGH);
		delay(500);
		digitalWrite(testPinPWM, LOW);
		delay(500);
	}
	noInterrupts();
	delay(1000);
	interrupts();
}

#endif


#if 0

#define LED_PIN D7
#define PWM_PIN D0
#define PWM_FRE_PIN D2

void setup()
{
    DEBUG("PWM Test\r\n");
    pinMode(LED_PIN, OUTPUT);
    pinMode(PWM_PIN, OUTPUT);
    pinMode(PWM_FRE_PIN, OUTPUT);
    uint32_t duty  = 125;
    uint32_t freq = 300;
    analogWrite(PWM_PIN, duty);
    analogWrite(PWM_FRE_PIN, duty, freq);
    //analogWrite(PWM_FRE_PIN, duty);
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(5000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(5000);                   // wait for a second
    DEBUG("Runing !");
}
#endif

#if 0
#define LED_PIN  36
#define TEST_PIN A2


void setup()
{
    DEBUG("Tone Test\r\n");
   pinMode(LED_PIN, OUTPUT);
pinMode(TEST_PIN,INPUT_PULLUP);
Serial.begin(115200);
}


// the loop function runs over and over again forever
void loop()
{
//   if(digitalRead(TEST_PIN)== 1)
//    {
//        digitalWrite(LED_PIN,HIGH);
//    }
//    else
//    {
//        digitalWrite(LED_PIN,LOW);
//    }
     // digitalWrite(LED_PIN,LOW);
      delay(100);
     // digitalWrite(LED_PIN,HIGH);
      delay(1000);
      Serial.println("asdfgh");
}
#endif


#if 0

#define LED_PIN D7
#define SERVO_CONTROL_PIN    A0 //定义舵机控制引脚

Servo myservo;
void setup()
{
    DEBUG("Servo Test\r\n");
    pinMode(LED_PIN, OUTPUT);
    myservo.attach(SERVO_CONTROL_PIN);
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                   // wait for a second
    DEBUG("Runing !");
    myservo.write(30);
    delay(3000);
    myservo.write(140);
    delay(3000);
}
#endif

#if 0
#include "Adafruit_SSD1306.h"
#define SSD1307
// Hareware SPI
// MOSI SPI MOSI
// CLK SPI CLK

#define LED_PIN D7
// // the vcc 3.3v
#define OLED_DC     D3
#define OLED_CS     D1
#define OLED_RESET  D4
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);  // Hareware SPI

// #define OLED_MOSI   A7
// #define OLED_CLK    A5
// #define OLED_DC     D3
// #define OLED_CS     D1
// #define OLED_RESET  D4
// Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);//  Software SPI

// #define OLED_MOSI   D0
// #define OLED_CLK    D3
// #define OLED_DC     D3
// #define OLED_CS     D1
// #define OLED_RESET  D5
// Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);//  Software SPI
char temp[128] = {'a', 'b', 'c','3', '2', '1', '0'};

void setup()
{
    DEBUG("SPI Test\r\n");
    pinMode(LED_PIN, OUTPUT);
#ifdef SSD1307
    display.begin(SSD1306_SWITCHCAPVCC);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(temp);
    display.display();
    delay(1000);
    display.clearDisplay();
    display.display();
    DEBUG("SPI Test Setup end\r\n");
#else
    //SPI.setClockDivider(SPI_CLOCK_DIV64);
    SPI.setClockDivider(SPI_CLOCK_DIV128);
    SPI.begin();
#endif
}

// the loop function runs over and over again forever
void loop()
{
    DEBUG("Runing !");
#ifdef SSD1307
    display.setTextSize(0);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(temp);
    display.display();
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                   // wait for a second
#else
    SPI.transfer(0xAA);
    SPI.transfer(0x55);
    SPI.transfer(0x00);
    SPI.transfer(0xFF);
    delay(100);
#endif
}
#endif

#if 0
#include <time.h>
#define LED_PIN D7

void setup()
{
    DEBUG("RTC Test\r\n");
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
}

// the loop function runs over and over again forever
void loop()
{
    //digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    //delay(1000);                   // wait for a second
    //digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    //delay(1000);                   // wait for a second

    DEBUG("\r\n");
    DEBUG("\r\n");
    DEBUG("\r\n");
    DEBUG("Runing !");

    delay(3000);
#if 1
    DEBUG("time   : %d", Time.now() );
    DEBUG("year   : %d", Time.year() );
    DEBUG("month  : %d", Time.month() );
    DEBUG("day    : %d", Time.day() );
    DEBUG("hour   : %d", Time.hour() );
    DEBUG("minute : %d", Time.minute() );
    DEBUG("second : %d", Time.second() );
    DEBUG("weekday: %d", Time.weekday() );
#endif

    DEBUG("\r\n");
    /*Time.setTime(0);*/
    // Time.setTime(1451606400UL); //20160101000000
    // Time.setTime(1454284800UL); //20160201000000
    delay(1000);
#if 0
    DEBUG("time   : %d", Time.now() );
    DEBUG("year   : %d", Time.year() );
    DEBUG("month  : %d", Time.month() );
    DEBUG("day    : %d", Time.day() );
    DEBUG("hour   : %d", Time.hour() );
    DEBUG("minute : %d", Time.minute() );
    DEBUG("second : %d", Time.second() );
    DEBUG("weekday: %d", Time.weekday() );

    struct tm tmstruct;
    tmstruct.tm_year = 1970 - 1900;
    tmstruct.tm_mon  = 1 - 1;
    tmstruct.tm_mday = 1;
    tmstruct.tm_hour = 0;
    tmstruct.tm_min  = 0;
    tmstruct.tm_sec  = 0;
    time_t t = mktime(&tmstruct);
    DEBUG("\r\n");
    DEBUG("time   : %d", t );
    struct tm *calend = gmtime(&t);
    DEBUG("year   : %d", calend->tm_year + 1900 );
    DEBUG("month  : %d", calend->tm_mon + 1);
    DEBUG("day    : %d", calend->tm_mday );
    DEBUG("hour   : %d", calend->tm_hour );
    DEBUG("minute : %d", calend->tm_min );
    DEBUG("second : %d", calend->tm_sec );
    DEBUG("\r\n");
    delay(1000);
#endif
}
#endif

#if 0

// ALL_LEVEL, TRACE_LEVEL, DEBUG_LEVEL, WARN_LEVEL, ERROR_LEVEL, PANIC_LEVEL, NO_LOG_LEVEL
Serial1DebugOutput debugOutput(115200, ALL_LEVEL);


#define LED_PIN D7

void setup()
{
    DEBUG("I2S Test\r\n");
    pinMode(LED_PIN, OUTPUT);
    //Audio.begin();
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                   // wait for a second
    DEBUG("Runing !");
    //Audio.receive();
}
#endif


#if 0

#define LED_PIN D6

void setup()
{
    DEBUG("Nut GPIO Test\r\n");
    pinMode(LED_PIN, OUTPUT);
    //Audio.begin();
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                   // wait for a second
    DEBUG("Runing !");
    //Audio.receive();
}
#endif
////////////////////////////////////Nut test examples///////////////////////////////////

#if 0
#define LED_PIN D6
void setup()
{
    DEBUG("Nut analogRead digitalRead Test\r\n");
    pinMode(LED_PIN, OUTPUT);
    pinMode(D1, INPUT);
    pinMode(A0, INPUT);
}
// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    //DEBUG("%d", digitalRead(D1));
    delay(1000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                   // wait for a second
    DEBUG("Runing !");
    uint16_t ad_value = analogRead(A0);
    DEBUG("%d", ad_value);
}
#endif

#if 0
#define LED_PIN D7
void setup()
{
    DEBUG("Nut AnalogRead Test\r\n");
    pinMode(LED_PIN, OUTPUT);
}
// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    //DEBUG("%d", digitalRead(D1));
    delay(1000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                   // wait for a second
    DEBUG("Runing !");
}
#endif

#if 0

#define LED_PIN D6
#define PWM_PIN D1
#define PWM_FRE_PIN D2

uint32_t duty  = 125;
uint32_t freq = 300;
void setup()
{
    DEBUG("Nut analogWrite Test\r\n");
    pinMode(LED_PIN, OUTPUT);
    pinMode(PWM_PIN, OUTPUT);
    pinMode(PWM_FRE_PIN, OUTPUT);
    analogWrite(PWM_PIN, duty);
    //analogWrite(PWM_FRE_PIN, duty, freq);
    //analogWrite(PWM_FRE_PIN, duty);
}

// the loop function runs over and over again forever
void loop()
{
    analogWrite(PWM_PIN, duty);
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(3000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(3000);                   // wait for a second
    DEBUG("Runing !");
}
#endif


#if 0
#include "gy30.h"

#define LED_PIN D6

GY30 gy30;
void setup()
{
    DEBUG("I2C Test\r\n");
    pinMode(LED_PIN, OUTPUT);
    gy30.begin();
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                   // wait for a second
    digitalWrite(LED_PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                   // wait for a second
    DEBUG("GY30 value: %f", gy30.Read());
    //DEBUG("Runing !");
}
#endif

#if 0
#define LED_PIN  D6
#define TEST_PIN D1

void setup()
{
    DEBUG("Nut Tone Test\r\n");
    pinMode(LED_PIN, OUTPUT);
    pinMode(TEST_PIN, OUTPUT);
    tone(TEST_PIN, 1000, 10000);
}


// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
}
#endif

#if 0
#define LED_PIN  D6
#define TEST_PIN D1

void setup()
{
    DEBUG("Nut timer hal millis micro Test\r\n");
    pinMode(LED_PIN, OUTPUT);
    pinMode(TEST_PIN, OUTPUT);
    //tone(TEST_PIN, 1000, 10000);
}


// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_PIN, LOW);
    delay(1000);
    DEBUG("micros: %ld", micros());
    DEBUG("millis: %ld", millis());
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
}
#endif


