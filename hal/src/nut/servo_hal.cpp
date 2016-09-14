/**
******************************************************************************
* @file    servo_hal.c
* @author  Matthew McGowan
* @version V1.0.0
* @date    27-Sept-2014
* @brief
******************************************************************************
Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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

/* Includes ------------------------------------------------------------------*/
#include "servo_hal.h"
// #include "ServoTimers.h"
#include "pinmap_impl.h"
#include "Arduino_Nut.h"
#include "Servo.h"

#if 0
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern void __pinMode(uint8_t pin, uint8_t mode) {
    if(pin < 16){
        if(mode == SPECIAL){
            GPC(pin) = (GPC(pin) & (0xF << GPCI)); //SOURCE(GPIO) | DRIVER(NORMAL) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
            GPEC = (1 << pin); //Disable
            GPF(pin) = GPFFS(GPFFS_BUS(pin));//Set mode to BUS (RX0, TX0, TX1, SPI, HSPI or CLK depending in the pin)
            if(pin == 3) GPF(pin) |= (1 << GPFPU);//enable pullup on RX
        } else if(mode & FUNCTION_0){
            GPC(pin) = (GPC(pin) & (0xF << GPCI)); //SOURCE(GPIO) | DRIVER(NORMAL) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
            GPEC = (1 << pin); //Disable
            GPF(pin) = GPFFS((mode >> 4) & 0x07);
            if(pin == 13 && mode == FUNCTION_4) GPF(pin) |= (1 << GPFPU);//enable pullup on RX
        }  else if(mode == OUTPUT || mode == OUTPUT_OPEN_DRAIN){
            GPF(pin) = GPFFS(GPFFS_GPIO(pin));//Set mode to GPIO
            GPC(pin) = (GPC(pin) & (0xF << GPCI)); //SOURCE(GPIO) | DRIVER(NORMAL) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
            if(mode == OUTPUT_OPEN_DRAIN) GPC(pin) |= (1 << GPCD);
            GPES = (1 << pin); //Enable
        } else if(mode == INPUT || mode == INPUT_PULLUP){
            GPF(pin) = GPFFS(GPFFS_GPIO(pin));//Set mode to GPIO
            GPEC = (1 << pin); //Disable
            GPC(pin) = (GPC(pin) & (0xF << GPCI)) | (1 << GPCD); //SOURCE(GPIO) | DRIVER(OPEN_DRAIN) | INT_TYPE(UNCHANGED) | WAKEUP_ENABLE(DISABLED)
            if(mode == INPUT_PULLUP) {
                GPF(pin) |= (1 << GPFPU);  // Enable  Pullup
            }
        } else if(mode == WAKEUP_PULLUP || mode == WAKEUP_PULLDOWN){
            GPF(pin) = GPFFS(GPFFS_GPIO(pin));//Set mode to GPIO
            GPEC = (1 << pin); //Disable
            if(mode == WAKEUP_PULLUP) {
                GPF(pin) |= (1 << GPFPU);  // Enable  Pullup
                GPC(pin) = (1 << GPCD) | (4 << GPCI) | (1 << GPCWE); //SOURCE(GPIO) | DRIVER(OPEN_DRAIN) | INT_TYPE(LOW) | WAKEUP_ENABLE(ENABLED)
            } else {
                GPF(pin) |= (1 << GPFPD);  // Enable  Pulldown
                GPC(pin) = (1 << GPCD) | (5 << GPCI) | (1 << GPCWE); //SOURCE(GPIO) | DRIVER(OPEN_DRAIN) | INT_TYPE(HIGH) | WAKEUP_ENABLE(ENABLED)
            }
        }
    } else if(pin == 16){
        GPF16 = GP16FFS(GPFFS_GPIO(pin));//Set mode to GPIO
        GPC16 = 0;
        if(mode == INPUT || mode == INPUT_PULLDOWN_16){
            if(mode == INPUT_PULLDOWN_16){
                GPF16 |= (1 << GP16FPD);//Enable Pulldown
            }
            GP16E &= ~1;
        } else if(mode == OUTPUT){
            GP16E |= 1;
        }
    }
}

extern void ICACHE_RAM_ATTR __digitalWrite(uint8_t pin, uint8_t val) {
    if(pin < 16){
        if(val) GPOS = (1 << pin);
        else GPOC = (1 << pin);
    } else if(pin == 16){
        if(val) GP16O |= 1;
        else GP16O &= ~1;
    }
}

extern int ICACHE_RAM_ATTR __digitalRead(uint8_t pin) {
    if(pin < 16){
        return GPIP(pin);
    } else if(pin == 16){
        return GP16I & 0x01;
    }
    return 0;
}
// the following are in us (microseconds)
//
#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000     // minumim time to refresh servos in microseconds

// NOTE: to maintain a strict refresh interval the user needs to not exceede 8 servos
#define SERVOS_PER_TIMER       12     // the maximum number of servos controlled by one timer
#define MAX_SERVOS   (ServoTimerSequence_COUNT  * SERVOS_PER_TIMER)

#define INVALID_SERVO         255     // flag indicating an invalid servo index

const uint32_t c_CycleCompensation = 4;  // compensation us to trim adjust for digitalWrite delays

#define INVALID_PIN           63    // flag indicating never attached servo

typedef struct _ServoInfo  {
    uint8_t pin : 6;             // a pin number from 0 to 62, 63 reserved
    uint8_t isActive : 1;        // true if this channel is enabled, pin not pulsed if false
    uint8_t isDetaching : 1;     // true if this channel is being detached, maintains pulse integrity
} ServoInfo;

struct ServoState {
    ServoInfo info;
    volatile uint16_t usPulse;
};

#if !defined (SERVO_EXCLUDE_TIMER0)
ServoTimer0 s_servoTimer0;
#endif

#if !defined (SERVO_EXCLUDE_TIMER1)
ServoTimer1 s_servoTimer1;
#endif

static ServoState s_servos[MAX_SERVOS];     // static array of servo structures

static uint8_t s_servoCount = 0;            // the total number of attached s_servos


// inconvenience macros
#define SERVO_INDEX_TO_TIMER(servoIndex) ((ServoTimerSequence)(servoIndex / SERVOS_PER_TIMER)) // returns the timer controlling this servo
#define SERVO_INDEX(timerId, channel)  ((timerId * SERVOS_PER_TIMER) + channel)     // macro to access servo index by timer and channel

// similiar to map but will have increased accuracy that provides a more
// symetric api (call it and use result to reverse will provide the original value)
//
int improved_map(int value, int minIn, int maxIn, int minOut, int maxOut)
{
    const int rangeIn = maxIn - minIn;
    const int rangeOut = maxOut - minOut;
    const int deltaIn = value - minIn;
    // fixed point math constants to improve accurancy of divide and rounding
    const int fixedHalfDecimal = 1;
    const int fixedDecimal = fixedHalfDecimal * 2;

    return ((deltaIn * rangeOut * fixedDecimal) / (rangeIn) + fixedHalfDecimal) / fixedDecimal + minOut;
}

//------------------------------------------------------------------------------
// Interrupt handler template method that takes a class that implements
// a standard set of methods for the timer abstraction
//------------------------------------------------------------------------------
//template <class T>
static void Servo_Handler0(ServoTimer0* timer) ICACHE_RAM_ATTR;

//template <class T>
static void Servo_Handler0(ServoTimer0* timer)
{
    uint8_t servoIndex;

    // clear interrupt
    timer->ResetInterrupt();

    if (timer->isEndOfCycle()) {
        timer->StartCycle();
    }
    else {
        servoIndex = SERVO_INDEX(timer->timerId(), timer->getCurrentChannel());
        if (servoIndex < s_servoCount && s_servos[servoIndex].info.isActive) {
            // pulse channel low if activated
            __digitalWrite(s_servos[servoIndex].info.pin, LOW);

            if (s_servos[servoIndex].info.isDetaching) {
                s_servos[servoIndex].info.isActive = false;
                s_servos[servoIndex].info.isDetaching = false;
            }
        }
        timer->nextChannel();
    }

    servoIndex = SERVO_INDEX(timer->timerId(), timer->getCurrentChannel());

    if (servoIndex < s_servoCount &&
        timer->getCurrentChannel() < SERVOS_PER_TIMER) {
        timer->SetPulseCompare(timer->usToTicks(s_servos[servoIndex].usPulse) - c_CycleCompensation);

        if (s_servos[servoIndex].info.isActive) {
            if (s_servos[servoIndex].info.isDetaching) {
                // it was active, reset state and leave low
                s_servos[servoIndex].info.isActive = false;
                s_servos[servoIndex].info.isDetaching = false;
            }
            else {
                // its an active channel so pulse it high, TODO
                __digitalWrite(s_servos[servoIndex].info.pin, HIGH);
            }
        }
    }
    else {
        if (!isTimerActive(timer->timerId())) {
            // no active running channels on this timer, stop the ISR
            finISR(timer->timerId());
        }
        else {
            // finished all channels so wait for the refresh period to expire before starting over
            // allow a few ticks to ensure the next match is not missed
            uint32_t refreshCompare = timer->usToTicks(REFRESH_INTERVAL);
            if ((timer->GetCycleCount() + c_CycleCompensation * 2) < refreshCompare) {
                timer->SetCycleCompare(refreshCompare - c_CycleCompensation);
            }
            else {
                // at least REFRESH_INTERVAL has elapsed
                timer->SetCycleCompare(timer->GetCycleCount() + c_CycleCompensation * 2);
            }
        }

        timer->setEndOfCycle();
    }
}

//template <class T>
static void Servo_Handler1(ServoTimer1* timer) ICACHE_RAM_ATTR;

//template <class T>
static void Servo_Handler1(ServoTimer1* timer)
{
    uint8_t servoIndex;

    // clear interrupt
    timer->ResetInterrupt();

    if (timer->isEndOfCycle()) {
        timer->StartCycle();
    }
    else {
        servoIndex = SERVO_INDEX(timer->timerId(), timer->getCurrentChannel());
        if (servoIndex < s_servoCount && s_servos[servoIndex].info.isActive) {
            // pulse channel low if activated
            __digitalWrite(s_servos[servoIndex].info.pin, LOW);

            if (s_servos[servoIndex].info.isDetaching) {
                s_servos[servoIndex].info.isActive = false;
                s_servos[servoIndex].info.isDetaching = false;
            }
        }
        timer->nextChannel();
    }

    servoIndex = SERVO_INDEX(timer->timerId(), timer->getCurrentChannel());

    if (servoIndex < s_servoCount &&
        timer->getCurrentChannel() < SERVOS_PER_TIMER) {
        timer->SetPulseCompare(timer->usToTicks(s_servos[servoIndex].usPulse) - c_CycleCompensation);

        if (s_servos[servoIndex].info.isActive) {
            if (s_servos[servoIndex].info.isDetaching) {
                // it was active, reset state and leave low
                s_servos[servoIndex].info.isActive = false;
                s_servos[servoIndex].info.isDetaching = false;
            }
            else {
                // its an active channel so pulse it high TODO
                __digitalWrite(s_servos[servoIndex].info.pin, HIGH);
            }
        }
    }
    else {
        if (!isTimerActive(timer->timerId())) {
            // no active running channels on this timer, stop the ISR
            finISR(timer->timerId());
        }
        else {
            // finished all channels so wait for the refresh period to expire before starting over
            // allow a few ticks to ensure the next match is not missed
            uint32_t refreshCompare = timer->usToTicks(REFRESH_INTERVAL);
            if ((timer->GetCycleCount() + c_CycleCompensation * 2) < refreshCompare) {
                timer->SetCycleCompare(refreshCompare - c_CycleCompensation);
            }
            else {
                // at least REFRESH_INTERVAL has elapsed
                timer->SetCycleCompare(timer->GetCycleCount() + c_CycleCompensation * 2);
            }
        }

        timer->setEndOfCycle();
    }
}

static void handler0() ICACHE_RAM_ATTR;
static void handler0()
{
    Servo_Handler0(&s_servoTimer0);
}

static void handler1() ICACHE_RAM_ATTR;
static void handler1()
{
    Servo_Handler1(&s_servoTimer1);
}

static void initISR(ServoTimerSequence timerId)
{
#if !defined (SERVO_EXCLUDE_TIMER0)
    if (timerId == ServoTimerSequence_Timer0)
        s_servoTimer0.InitInterrupt(&handler0);
#endif
#if !defined (SERVO_EXCLUDE_TIMER1)
    if (timerId == ServoTimerSequence_Timer1)
        s_servoTimer1.InitInterrupt(&handler1);
#endif
}

static void finISR(ServoTimerSequence timerId) ICACHE_RAM_ATTR;
static void finISR(ServoTimerSequence timerId)
{
#if !defined (SERVO_EXCLUDE_TIMER0)
    if (timerId == ServoTimerSequence_Timer0)
        s_servoTimer0.StopInterrupt();
#endif
#if !defined (SERVO_EXCLUDE_TIMER1)
    if (timerId == ServoTimerSequence_Timer1)
        s_servoTimer1.StopInterrupt();
#endif
}

// returns true if any servo is active on this timer
// static boolean isTimerActive(ServoTimerSequence timerId) ICACHE_RAM_ATTR;
// static boolean isTimerActive(ServoTimerSequence timerId)
static bool isTimerActive(ServoTimerSequence timerId) ICACHE_RAM_ATTR;
static bool isTimerActive(ServoTimerSequence timerId)
{
    for (uint8_t channel = 0; channel < SERVOS_PER_TIMER; channel++) {
        if (s_servos[SERVO_INDEX(timerId, channel)].info.isActive) {
            return true;
        }
    }
    return false;
}

uint8_t _servoIndex;
uint16_t _minUs;
uint16_t _maxUs;
void checkServo(void)
{
    if (s_servoCount < MAX_SERVOS) {
        // assign a servo index to this instance
        _servoIndex = s_servoCount++;
        // store default values
        s_servos[_servoIndex].usPulse = DEFAULT_PULSE_WIDTH;

        // set default _minUs and _maxUs incase write() is called before attach()
        _minUs = MIN_PULSE_WIDTH;
        _maxUs = MAX_PULSE_WIDTH;

        s_servos[_servoIndex].info.isActive = false;
        s_servos[_servoIndex].info.isDetaching = false;
        s_servos[_servoIndex].info.pin = INVALID_PIN;
    }
    else {
        _servoIndex = INVALID_SERVO;  // too many servos
    }
}


void HAL_Servo_Attach(uint16_t pin)
{
    EESP82666_Pin_Info* PIN_MAP = HAL_Pin_Map();

    pin_t gpio_pin = PIN_MAP[pin].gpio_pin;

    checkServo();
    ServoTimerSequence timerId;

    if (_servoIndex < MAX_SERVOS) {
        if (s_servos[_servoIndex].info.pin == INVALID_PIN) {
            __pinMode(gpio_pin, OUTPUT);       // set servo pin to output
            __digitalWrite(gpio_pin, LOW);
            s_servos[_servoIndex].info.pin = gpio_pin;
        }

        // keep the min and max within 200-3000 us, these are extreme
        // ranges and should support extreme servos while maintaining
        // reasonable ranges
        _maxUs = max(250, min(3000, MAX_PULSE_WIDTH));
        _minUs = max(200, min(_maxUs, MIN_PULSE_WIDTH));

        // initialize the timerId if it has not already been initialized
        timerId = SERVO_INDEX_TO_TIMER(_servoIndex);
        if (!isTimerActive(timerId)) {
            initISR(timerId);
        }
        s_servos[_servoIndex].info.isDetaching = false;
        s_servos[_servoIndex].info.isActive = true;  // this must be set after the check for isTimerActive
    }
    //return _servoIndex;

}

void HAL_Servo_Detach(uint16_t pin)
{
    ServoTimerSequence timerId;

    if (s_servos[_servoIndex].info.isActive) {
        s_servos[_servoIndex].info.isDetaching = true;
    }

}

void HAL_Servo_Write_Pulse_Width(uint16_t pin, uint16_t pulseWidth)
{
    // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
    if (pulseWidth < MIN_PULSE_WIDTH) {
        // assumed to be 0-180 degrees servo
        pulseWidth = constrain(pulseWidth, 0, 180);
        // writeMicroseconds will contrain the calculated value for us
        // for any user defined min and max, but we must use default min max
        pulseWidth = improved_map(pulseWidth, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    }
    // ensure channel is valid
    if ((_servoIndex < MAX_SERVOS)) {
        // ensure pulse width is valid
        pulseWidth = constrain(pulseWidth, _minUs, _maxUs);

        s_servos[_servoIndex].usPulse = pulseWidth;
    }
}

uint16_t HAL_Servo_Read_Pulse_Width(uint16_t pin)
{
    unsigned int pulsewidth;
    if (_servoIndex != INVALID_SERVO) {
        pulsewidth = s_servos[_servoIndex].usPulse;
    }
    else {
        pulsewidth = 0;
    }

    return pulsewidth;
}

uint16_t HAL_Servo_Read_Frequency(uint16_t pin)
{
    return 0;
}
#endif

#define MAX_SERVOS_NUM 7

static ServoHal* myServo[MAX_SERVOS_NUM];

typedef struct pinInfo {
    uint16_t pin;
    bool isActive;
}pinInfo;

// only 4 pwm pins here: D1 D2 D3 D5
pinInfo PinServoIndex[MAX_SERVOS_NUM] = {
    {D0, false}
    ,{D1, false}
    ,{D2, false}
    ,{D3, false}
    ,{D4, false}
    ,{D5, false}
    ,{D6, false}
};

int8_t findServoIndex(uint16_t pin)
{
    for (int i = 0; i< MAX_SERVOS_NUM; i++) {
        if (PinServoIndex[i].pin == pin) {
            return i;
        }
    }
    return -1;
}


void HAL_Servo_Attach(uint16_t pin)
{
    int8_t pin_severindex = findServoIndex(pin);
    if (pin_severindex != -1) {
        EESP82666_Pin_Info* PIN_MAP = HAL_Pin_Map();
        pin_t gpio_pin = PIN_MAP[pin].gpio_pin;
        if (PinServoIndex[pin_severindex].isActive == false) {
            myServo[pin_severindex] = new ServoHal();
            PinServoIndex[pin_severindex].isActive = true;
        }
        myServo[pin_severindex]->attach(gpio_pin);
    }
}

void HAL_Servo_Detach(uint16_t pin)
{
    int8_t pin_severindex = findServoIndex(pin);
    myServo[pin_severindex]->detach();
}

void HAL_Servo_Write_Pulse_Width(uint16_t pin, uint16_t pulseWidth)
{
    int8_t pin_severindex = findServoIndex(pin);
    myServo[pin_severindex]->write(pulseWidth);
}

uint16_t HAL_Servo_Read_Pulse_Width(uint16_t pin)
{
    int8_t pin_severindex = findServoIndex(pin);
    return myServo[pin_severindex]->read();
}

uint16_t HAL_Servo_Read_Frequency(uint16_t pin)
{
    return 0;
}
