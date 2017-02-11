/**
 ******************************************************************************
 * @file    serial_test.cpp
 * @authors  lbz
 * @version V1.0.0
 * @date    2014_11_13
 * @brief   
 ******************************************************************************
*/

#include "application.h"



static int led_test = 13;  
static int led_test1 = 11; 

//static unsigned int count_led = 0;
 
void setup_DHT00_lib_public_test1()
{  
	pinMode(led_test, OUTPUT);  
    pinMode(led_test1, OUTPUT);  

}


void loop_DHT00_lib_public_test1()
{
//	if(count_led==0)
//	{
		digitalWrite(led_test, LOW);   // Turn ON the LED pins  
		digitalWrite(led_test1, LOW);  
        //delayMicroseconds(100000);
//	}

		delay(400);
//	if(count_led==50000)
//	{
		digitalWrite(led_test, HIGH);   // Turn ON the LED pins  
		digitalWrite(led_test1, HIGH); 
         //delayMicroseconds(100000);
//	}

    
	delay(400);
//	count_led++; 
//	if(count_led==100000) count_led=0;

}
