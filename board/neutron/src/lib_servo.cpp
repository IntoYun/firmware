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

#define SERVO_TIM_PWM_FREQ	50                                              //20ms = 50Hz
#define SERVO_TIM_PRESCALER	(uint16_t)(SystemCoreClock / 1000000) - 1		//To get TIM counter clock = 1MHz
#define SERVO_TIM_ARR		(uint16_t)(1000000 / SERVO_TIM_PWM_FREQ) - 1	//To get PWM period = 20ms

#define ANGLE_TO_US(a)    ((uint16_t)(map((a), this->minAngle, this->maxAngle, \
                                          this->minPW, this->maxPW)))
#define US_TO_ANGLE(us)   ((int16_t)(map((us), this->minPW, this->maxPW, \
                                         this->minAngle, this->maxAngle)))

/*********************************************************************************
 *Function		:     Servo::Servo(void)
 *Description	:     constructor function
 *Input              :     none
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
                            *Input              :  pin:Pin connected to the servo pulse wave input. This pin must be capable of PWM output.
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
    if(PIN_MAP[pin].timer_peripheral != NULL || PIN_MAP[pin].timer_peripheral != TIM1)
    {
        if( (SPI.isEnabled() == true ) && ( (pin == PIN_SPI_SCK) || (pin == PIN_SPI_MISO) || (pin == PIN_SPI_MOSI) ))
        {
            // Error
        }

        if( (Serial.isEnabled() == true) && ( (pin == RX) || (pin == TX) ))
        {
            // Error
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

        /* Common configruation for all channels */
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;


        pinMode(pin, AF_OUTPUT_PUSHPULL);

        // TIM clock enable

        // if(PIN_MAP[pin].timer_peripheral == TIM1)
        // {
        //     __HAL_RCC_TIM1_CLK_ENABLE();
        //     GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        //     GPIO_InitStruct.Pin       = PIN_MAP[pin].gpio_pin;
        //     if( (PIN_MAP[pin].gpio_peripheral == GPIOA) )
        //     {
        //         __HAL_RCC_GPIOA_CLK_ENABLE();
        //         HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        //     }
        //     else if( (PIN_MAP[pin].gpio_peripheral == GPIOB) )
        //     {
        //         __HAL_RCC_GPIOB_CLK_ENABLE();
        //         HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        //     }
        // }
        // else if(PIN_MAP[pin].timer_peripheral == TIM2)
        if(PIN_MAP[pin].timer_peripheral == TIM2)
        {
            __HAL_RCC_TIM2_CLK_ENABLE();
            GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
            GPIO_InitStruct.Pin       = PIN_MAP[pin].gpio_pin;
            if( (PIN_MAP[pin].gpio_peripheral == GPIOA) )
            {
                __HAL_RCC_GPIOA_CLK_ENABLE();
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            }
            else if( (PIN_MAP[pin].gpio_peripheral == GPIOB) )
            {
                __HAL_RCC_GPIOB_CLK_ENABLE();
                HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            }
        }
        else if(PIN_MAP[pin].timer_peripheral == TIM3)
        {
            __HAL_RCC_TIM3_CLK_ENABLE();
            GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
            GPIO_InitStruct.Pin       = PIN_MAP[pin].gpio_pin;
            if( (PIN_MAP[pin].gpio_peripheral == GPIOA) )
            {
                __HAL_RCC_GPIOA_CLK_ENABLE();
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            }
            else if( (PIN_MAP[pin].gpio_peripheral == GPIOB) )
            {
                __HAL_RCC_GPIOB_CLK_ENABLE();
                HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            }
        }
        else if(PIN_MAP[pin].timer_peripheral == TIM4)
        {
            __HAL_RCC_TIM4_CLK_ENABLE();
            GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
            GPIO_InitStruct.Pin       = PIN_MAP[pin].gpio_pin;
            if( (PIN_MAP[pin].gpio_peripheral == GPIOA) )
            {
                __HAL_RCC_GPIOA_CLK_ENABLE();
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            }
            else if( (PIN_MAP[pin].gpio_peripheral == GPIOB) )
            {
                __HAL_RCC_GPIOB_CLK_ENABLE();
                HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            }
        }
        else if(PIN_MAP[pin].timer_peripheral == TIM5)
        {

            __HAL_RCC_TIM5_CLK_ENABLE();
            GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
            GPIO_InitStruct.Pin       = PIN_MAP[pin].gpio_pin;
            if( (PIN_MAP[pin].gpio_peripheral == GPIOA) )
            {
                __HAL_RCC_GPIOA_CLK_ENABLE();
                HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            }
            else if( (PIN_MAP[pin].gpio_peripheral == GPIOB) )
            {
                __HAL_RCC_GPIOB_CLK_ENABLE();
                HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            }
        }


        TIM_HandleTypeDef TimHandle;
        TIM_OC_InitTypeDef sConfig;

        TimHandle.Instance = PIN_MAP[pin].timer_peripheral;

        TimHandle.Init.Prescaler         = SERVO_TIM_PRESCALER;
        TimHandle.Init.Period            = SERVO_TIM_ARR;
        TimHandle.Init.ClockDivision     = 0;
        TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
        TimHandle.Init.RepetitionCounter = 0;
        if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
        {
            // Error
        }

        sConfig.OCMode       = TIM_OCMODE_PWM1;
        sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
        sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
        sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;

        // if(PIN_MAP[pin].timer_peripheral == TIM1)
        // {
        //     sConfig.OCIdleState  = TIM_OCIDLESTATE_SET;
        //     sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
        // }

       /* Set the pulse value for channel 1 */
        sConfig.Pulse = 0x00; // no pwm at start point
        if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, PIN_MAP[pin].timer_ch) != HAL_OK)
        {
            /* Configuration Error */
            //Error_Handler();
        }

        /* Start channel */
        if (HAL_TIM_PWM_Start(&TimHandle, PIN_MAP[pin].timer_ch) != HAL_OK)
        {
            /* PWM Generation Error */
            //Error_Handler();
        }

        return true;
    }
    else
    {
        return false;
    }
}

/*********************************************************************************
 *Function		: bool detach(void)
 *Description	: Stop driving the servo pulse train. If not currently attached to a motor, this function has no effect.
 *Input              : none
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
    TIM_HandleTypeDef TimHandle;

    if(PIN_MAP[pin].timer_peripheral == TIM2)
        TimHandle.Instance = TIM2;
    else if(PIN_MAP[pin].timer_peripheral == TIM3)
        TimHandle.Instance = TIM3;
    else if(PIN_MAP[pin].timer_peripheral == TIM4)
        TimHandle.Instance = TIM4;
    else if(PIN_MAP[pin].timer_peripheral == TIM5)
        TimHandle.Instance = TIM5;
    // else if(PIN_MAP[pin].timer_peripheral == TIM1)
    //     TimHandle.Instance = TIM1;

    // TIM_Cmd(PIN_MAP[this->pin].timer_peripheral, DISABLE);
    //__HAL_TIM_DISABLE(&TimHandle);
    HAL_TIM_PWM_Stop(&TimHandle,TIM_CHANNEL_ALL);

    this->resetFields();
    return true;
}

/*********************************************************************************
 *Function		: bool attached(void) const
 *Description	: Check if this instance is attached to a servo.
 *Input              : none
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
 *Input              : none
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
                            *Input              : degrees: value
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
 *Input              : none
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
                             *Input              : pulseWidth: the pulseWidth value
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

    TIM_OC_InitTypeDef sConfig;

    sConfig.OCMode       = TIM_OCMODE_PWM1;
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;



    // if(PIN_MAP[pin].timer_peripheral == TIM1)
    // {
    //     sConfig.OCIdleState  = TIM_OCIDLESTATE_SET;
    //     sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    // }

    /* Set the pulse value for channel 1 */
    sConfig.Pulse = SERVO_TIM_CCR;

    TIM_HandleTypeDef TimHandle;

    TimHandle.Instance = PIN_MAP[pin].timer_peripheral;

    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, PIN_MAP[pin].timer_ch) != HAL_OK)
    {
        /* Configuration Error */
        //Error_Handler();
    }

    /* Start channel */
    if (HAL_TIM_PWM_Start(&TimHandle, PIN_MAP[pin].timer_ch) != HAL_OK)
    {
        /* PWM Generation Error */
        //Error_Handler();
    }
}

/*********************************************************************************
 *Function		: uint16_t  readMicroseconds() const
 *Description	: Get the current pulse width, in microseconds.  This will lie within the range specified at attach() time.
 *Input              : none
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

    TIM_HandleTypeDef htim;
    htim.Instance = PIN_MAP[this->pin].timer_peripheral;

    SERVO_TIM_CCR = HAL_TIM_ReadCapturedValue(&htim, PIN_MAP[this->pin].timer_ch);

    //pulseWidth = (SERVO_TIM_CCR * 1000000) / ((SERVO_TIM_ARR + 1) * SERVO_TIM_PWM_FREQ);
    return SERVO_TIM_CCR;
}


/*********************************************************************************
 *Function		: void resetFields(void)
 *Description	: initialize
 *Input              : none
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
