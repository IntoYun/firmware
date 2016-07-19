/**
 ******************************************************************************
 * @file        : lib_servo.cpp
 * @author   : robin
 * @version  : V1.0.0
 * @date      : 2015-02-01
 * @brief      :   
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
 */
#include "lib_servo.h"
#include "wiring_usartserial.h"
#include "wiring_spi.h"
#include "wiring_i2c.h"

#define SERVO_TIM_PWM_FREQ	50											    //20ms = 50Hz
#define SERVO_TIM_PRESCALER	(uint16_t)(SystemCoreClock / 1000000) - 1		//To get TIM counter clock = 1MHz
#define SERVO_TIM_ARR		(uint16_t)(1000000 / SERVO_TIM_PWM_FREQ) - 1	//To get PWM period = 20ms

#define ANGLE_TO_US(a)    ((uint16_t)(map((a), this->minAngle, this->maxAngle, \
                                        this->minPW, this->maxPW)))
#define US_TO_ANGLE(us)   ((int16_t)(map((us), this->minPW, this->maxPW,  \
                                       this->minAngle, this->maxAngle)))
                                       
/*********************************************************************************
  *Function		:     Servo::Servo(void) 
  *Description	:     constructor function
  *Input		      :     none
  *Output		:     none    
  *Return		:     none
  *author		:     robot   
  *date			:     2015-02-01      
  *Others		:         
**********************************************************************************/
Servo::Servo(void) 
{
    this->resetFields();
}

/*********************************************************************************
  *Function		: bool attach      
  *Description	: Associate this instance with a servomotor whose input is connected to pin.
                            If this instance is already attached to a pin, it will be detached before being attached to the new pin. 
                            This function doesn't detach any interrupt attached with the pin's timer channel.
  *Input		      :  pin:Pin connected to the servo pulse wave input. This pin must be capable of PWM output.
                            minPulseWidth: Minimum pulse width to write to pin, in microseconds.  This will be associated with a minAngle degree angle.  
                                 Defaults to SERVO_DEFAULT_MIN_PW = 544.
                            maxPulseWidth:Maximum pulse width to write to pin, in microseconds.  This will be associated with a maxAngle degree angle. 
                                 Defaults to SERVO_DEFAULT_MAX_PW = 2400.
                            minAngle:Target angle (in degrees) associated with minPulseWidth.  
                                 Defaults to SERVO_DEFAULT_MIN_ANGLE = 0.
                            maxAngle:Target angle (in degrees) associated with maxPulseWidth.  
                                 Defaults to SERVO_DEFAULT_MAX_ANGLE = 180.
  *Output		: none       
  *Return		: return true if successful, false when pin doesn't support PWM.    
  *author		: lz       
  *date			: 10-December-2014          
  *Others		: none        
**********************************************************************************/
bool Servo::attach(uint16_t pin, uint16_t minPW, uint16_t maxPW, int16_t minAngle, int16_t maxAngle) 
{
    if (pin >= TOTAL_PINS || PIN_MAP[pin].timer_peripheral == NULL)
    {
        return false;
    }

    //  LZ 增加
    if(pin == 46 || pin == 47)
    {
        return false; // 46 47为 stm32与7620n 串口通讯 IO 用户不可操作
    }

    // SPI safety check
    if ((SPI.isEnabled() == true && (pin == PIN_SPI_SCK || pin == PIN_SPI_MOSI|| pin == PIN_SPI_MISO))
    ||(SPI_1.isEnabled() == true && (pin == PIN_SPI_SCK1 || pin == PIN_SPI_MOSI1 || pin == PIN_SPI_MISO1)))
    {
        return false;
    }

    // I2C safety check
    if ((Wire.isEnabled() == true && (pin == PIN_I2C_SCL || pin == PIN_I2C_SDA))
    ||(Wire1.isEnabled() == true && (pin == PIN_I2C_SCL1 || pin == PIN_I2C_SDA1)))
    {
        return false;
    }

    // Serial1 safety check
    if (Serial1.isEnabled() == true && (pin == RX || pin == TX))
    {
        return false;
    }

    // LZ 修改
    if (SerialBridge.isEnabled() == true && (pin == RX2 || pin == TX2))
    {
        return false;
    }

    if (Serial.isEnabled() == true && (pin == RX1 || pin == TX1))
    {
        return false;
    }

    if (this->attached())
    {
        this->detach();
    }

    this->pin = pin;
    this->minPW = minPW;
    this->maxPW = maxPW;
    this->minAngle = minAngle;
    this->maxAngle = maxAngle;

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    // AFIO clock enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    pinMode(pin, AF_OUTPUT_PUSHPULL);

    // TIM clock enable
    if(PIN_MAP[pin].timer_peripheral == TIM2)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    else if(PIN_MAP[pin].timer_peripheral == TIM3)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    else if(PIN_MAP[pin].timer_peripheral == TIM4)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    // Time base configuration
    TIM_TimeBaseStructure.TIM_Period = SERVO_TIM_ARR;
    TIM_TimeBaseStructure.TIM_Prescaler = SERVO_TIM_PRESCALER;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(PIN_MAP[pin].timer_peripheral, &TIM_TimeBaseStructure);

    // PWM1 Mode configuration
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0x0000;

    if(PIN_MAP[this->pin].timer_ch == TIM_Channel_1)
    {
        // PWM1 Mode configuration: Channel1
        TIM_OC1Init(PIN_MAP[this->pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(PIN_MAP[this->pin].timer_peripheral, TIM_OCPreload_Enable);
    }
    else if(PIN_MAP[this->pin].timer_ch == TIM_Channel_2)
    {
        // PWM1 Mode configuration: Channel2
        TIM_OC2Init(PIN_MAP[this->pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(PIN_MAP[this->pin].timer_peripheral, TIM_OCPreload_Enable);
    }
    else if(PIN_MAP[this->pin].timer_ch == TIM_Channel_3)
    {
        // PWM1 Mode configuration: Channel3
        TIM_OC3Init(PIN_MAP[this->pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC3PreloadConfig(PIN_MAP[this->pin].timer_peripheral, TIM_OCPreload_Enable);
    }
    else if(PIN_MAP[this->pin].timer_ch == TIM_Channel_4)
    {
        // PWM1 Mode configuration: Channel4
        TIM_OC4Init(PIN_MAP[this->pin].timer_peripheral, &TIM_OCInitStructure);
        TIM_OC4PreloadConfig(PIN_MAP[this->pin].timer_peripheral, TIM_OCPreload_Enable);
    }

    TIM_ARRPreloadConfig(PIN_MAP[this->pin].timer_peripheral, ENABLE);

    // TIM enable counter
    TIM_Cmd(PIN_MAP[this->pin].timer_peripheral, ENABLE);

    // Main Output Enable
    TIM_CtrlPWMOutputs(PIN_MAP[this->pin].timer_peripheral, ENABLE);

    return true;
}

/*********************************************************************************
  *Function		: bool detach(void)       
  *Description	: Stop driving the servo pulse train. If not currently attached to a motor, this function has no effect.
  *Input		      : none       
  *Output		: none    
  *Return		: return true if this call did anything, false otherwise.
  *author		: lz       
  *date			: 10-December-2014             
  *Others		:         
**********************************************************************************/
bool Servo::detach(void) 
{
    if (!this->attached()) 
    {
        return false;
    }

    // TIM disable counter
    TIM_Cmd(PIN_MAP[this->pin].timer_peripheral, DISABLE);
    this->resetFields();
    return true;
}

/*********************************************************************************
  *Function		: bool attached(void) const    
  *Description	: Check if this instance is attached to a servo.
  *Input		      : none       
  *Output		: none    
  *Return		: return true if this instance is attached to a servo, false otherwise.
  *author		: lz       
  *date			: 10-December-2014             
  *Others		:         
**********************************************************************************/
bool Servo::attached(void) const 
{ 
    return this->pin != NOT_ATTACHED; 
}

/*********************************************************************************
  *Function		: int attachedPin(void) const  
  *Description	: Get the pin this instance is attached to.
  *Input		      : none       
  *Output		: none    
  *Return		: return Pin number if currently attached to a pin, NOT_ATTACHED otherwise.
  *author		: lz       
  *date			: 10-December-2014             
  *Others		:         
**********************************************************************************/
int Servo::attachedPin(void) const 
{ 
    return this->pin; 
}

/*********************************************************************************
  *Function		: void write(int degrees)      
  *Description	: angle Target angle, in degrees.  
                            If the target angle is outside the range specified at attach() time, it will be clamped to lie in that range.
  *Input		      : degrees: value        
  *Output		: none        
  *Return		: none   
  *author		: lz   
  *data			: 10-December-2014         
  *Others		:         
**********************************************************************************/
void Servo::write(int degrees) 
{
    degrees = constrain(degrees, this->minAngle, this->maxAngle);
    this->writeMicroseconds(ANGLE_TO_US(degrees));
}

/*********************************************************************************
  *Function		: int read(void) const      
  *Description	: Get the servomotor's target angle, in degrees.  This will lie inside the range specified at attach() time.   
  *Input		      : none         
  *Output		: none    
  *Return		: the angle value    
  *author		: lz      
  *date			: 10-December-2014          
  *Others		:         
**********************************************************************************/
int Servo::read(void) const
{
    int a = US_TO_ANGLE(this->readMicroseconds());
    // map() round-trips in a weird way we mostly correct for here;
    // the round-trip is still sometimes off-by-one for write(1) and
    // write(179).
    return a == this->minAngle || a == this->maxAngle ? a : a + 1;
}

/*********************************************************************************
  *Function		: void writeMicroseconds(uint16_t pulseWidth)        
  *Description	:  pulseWidth Pulse width to send to the servomotor, in microseconds. 
                             If outside of the range specified at attach() time, it is clamped tolie in that range.
  *Input		      : pulseWidth: the pulseWidth value     
  *Output		: none        
  *Return		: none       
  *author		: lz       
  *date			: 10-December-2014            
  *Others		:         
**********************************************************************************/
void Servo::writeMicroseconds(uint16_t pulseWidth) 
{
    if (!this->attached()) 
    {
        return;
    }

    pulseWidth = constrain(pulseWidth, this->minPW, this->maxPW);

    //SERVO_TIM_CCR = pulseWidth * (SERVO_TIM_ARR + 1) * SERVO_TIM_PWM_FREQ / 1000000;
    uint16_t SERVO_TIM_CCR = pulseWidth;

    if(PIN_MAP[this->pin].timer_ch == TIM_Channel_1)
    {
        TIM_SetCompare1(PIN_MAP[this->pin].timer_peripheral, SERVO_TIM_CCR);
    }
    else if(PIN_MAP[this->pin].timer_ch == TIM_Channel_2)
    {
        TIM_SetCompare2(PIN_MAP[this->pin].timer_peripheral, SERVO_TIM_CCR);
    }
    else if(PIN_MAP[this->pin].timer_ch == TIM_Channel_3)
    {
        TIM_SetCompare3(PIN_MAP[this->pin].timer_peripheral, SERVO_TIM_CCR);
    }
    else if(PIN_MAP[this->pin].timer_ch == TIM_Channel_4)
    {
        TIM_SetCompare4(PIN_MAP[this->pin].timer_peripheral, SERVO_TIM_CCR);
    }
}

/*********************************************************************************
  *Function		: uint16_t  readMicroseconds() const         
  *Description	: Get the current pulse width, in microseconds.  This will lie within the range specified at attach() time.
  *Input		      : none     
  *Output		: none        
  *Return		: the value of current pulse width     
  *author		: lz       
  *date			: 10-December-2014            
  *Others		:         
**********************************************************************************/
uint16_t Servo::readMicroseconds(void) const
{
    if (!this->attached()) 
    {
        return 0;
    }

    uint16_t SERVO_TIM_CCR = 0x0000;

    if(PIN_MAP[this->pin].timer_ch == TIM_Channel_1)
    {
    	SERVO_TIM_CCR = TIM_GetCapture1(PIN_MAP[this->pin].timer_peripheral);
    }
    else if(PIN_MAP[this->pin].timer_ch == TIM_Channel_2)
    {
    	SERVO_TIM_CCR = TIM_GetCapture2(PIN_MAP[this->pin].timer_peripheral);
    }
    else if(PIN_MAP[this->pin].timer_ch == TIM_Channel_3)
    {
    	SERVO_TIM_CCR = TIM_GetCapture3(PIN_MAP[this->pin].timer_peripheral);
    }
    else if(PIN_MAP[this->pin].timer_ch == TIM_Channel_4)
    {
    	SERVO_TIM_CCR = TIM_GetCapture4(PIN_MAP[this->pin].timer_peripheral);
    }

    //pulseWidth = (SERVO_TIM_CCR * 1000000) / ((SERVO_TIM_ARR + 1) * SERVO_TIM_PWM_FREQ);
    return SERVO_TIM_CCR;
}


/*********************************************************************************
  *Function		: void resetFields(void)        
  *Description	: initialize  
  *Input		      : none        
  *Output		: none    
  *Return		: none   
  *author		: lz   
  *date			: 10-December-2014          
  *Others		:         
**********************************************************************************/
void Servo::resetFields(void) 
{
    this->pin = NOT_ATTACHED;
    this->minAngle = SERVO_DEFAULT_MIN_ANGLE;
    this->maxAngle = SERVO_DEFAULT_MAX_ANGLE;
    this->minPW = SERVO_DEFAULT_MIN_PW;
    this->maxPW = SERVO_DEFAULT_MAX_PW;
}

