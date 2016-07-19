/* Copyright (c) 2014 Paul Kourany, based on work by Dianel Gilbert

Copyright (c) 2013 Daniel Gilbert, loglow@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

```
PIN		TMR2	TMR3	TMR4
----------------------------
D6                       x
D7                       x
D8                       x
D9                       x

A0       x
A1       x
A2       x
A3       x

A6               x
A7               x
A8               x
A9               x
```

Above PWM timer output pins are pin by analogWrite () call output PWM, followed tone () can also be output PWM,
So use this library when the timer corresponding pins are not callable analogWrite () and tone (),
otherwise they will not get the results you want.

For example, with this library TIM2, you can not call analogWrite () and tone () in the D6-D9 pin, or waveform D6-D9 will not be what you want.
But if TIM3 and TIM4 not used, then the A0-A3, A6-A9 are not affected can still call analogWrite () and tone ().

*/
//Adapted for IntoRobot by Robin, Sept 19, 2015

#ifndef __WIRING_INTERVALTIMER_H__
#define __WIRING_INTERVALTIMER_H__

#include "application.h"


enum {uSec, hmSec};			// microseconds or half-milliseconds

#ifdef __cplusplus
extern "C" {
#endif

extern void (*Wiring_TIM2_Interrupt_Handler)(void);
extern void (*Wiring_TIM3_Interrupt_Handler)(void);
extern void (*Wiring_TIM4_Interrupt_Handler)(void);
extern void (*Wiring_TIM5_Interrupt_Handler)(void);
extern void (*Wiring_TIM9_Interrupt_Handler)(void);
extern void (*Wiring_TIM10_Interrupt_Handler)(void);
extern void (*Wiring_TIM11_Interrupt_Handler)(void);


extern void Wiring_TIM2_Interrupt_Handler_override(void);
extern void Wiring_TIM3_Interrupt_Handler_override(void);
extern void Wiring_TIM4_Interrupt_Handler_override(void);
extern void Wiring_TIM5_Interrupt_Handler_override(void);
extern void Wiring_TIM9_Interrupt_Handler_override(void);
extern void Wiring_TIM10_Interrupt_Handler_override(void);
extern void Wiring_TIM11_Interrupt_Handler_override(void);

enum action {INT_DISABLE, INT_ENABLE};
enum TIMid {TIMER2, TIMER3, TIMER4, TIMER5, TIMER9, TIMER10, TIMER11, AUTO=255};

class IntoRobotIntervalTimer {
  private:
    typedef void (*ISRcallback)();
    enum {TIMER_OFF, TIMER_SIT};
    static const uint8_t NUM_SIT = 7;

    // Timer ClockDivision = DIV4
    const uint16_t SIT_PRESCALERu = (uint16_t)(96000000 / 1000000) - 1;	//To get TIM counter clock = 1MHz
    const uint16_t SIT_PRESCALERm = (uint16_t)(96000000 / 2000) - 1;		//To get TIM counter clock = 1KHz
    const uint16_t MAX_PERIOD = 65535;//UINT16_MAX;		// 1-65535 us

    static bool SIT_used[NUM_SIT];
    bool allocate_SIT(uint16_t Period, bool scale, TIMid id);
    void start_SIT(uint16_t Period, bool scale);
    void stop_SIT();
    bool status;
    uint8_t SIT_id;
    ISRcallback myISRcallback;

    bool beginCycles(void (*isrCallback)(), uint16_t Period, bool scale, TIMid id);

  public:
    IntoRobotIntervalTimer() {
        status = TIMER_OFF;
        Wiring_TIM2_Interrupt_Handler = Wiring_TIM2_Interrupt_Handler_override;
        Wiring_TIM3_Interrupt_Handler = Wiring_TIM3_Interrupt_Handler_override;
        Wiring_TIM4_Interrupt_Handler = Wiring_TIM4_Interrupt_Handler_override;
        Wiring_TIM5_Interrupt_Handler = Wiring_TIM5_Interrupt_Handler_override;
        Wiring_TIM9_Interrupt_Handler = Wiring_TIM9_Interrupt_Handler_override;
        Wiring_TIM10_Interrupt_Handler = Wiring_TIM10_Interrupt_Handler_override;
        Wiring_TIM11_Interrupt_Handler = Wiring_TIM11_Interrupt_Handler_override;
    }
    ~IntoRobotIntervalTimer() { end(); }

    bool begin(void (*isrCallback)(), uint16_t Period, bool scale) {
        if (Period == 0 || Period > MAX_PERIOD)
            return false;
        return beginCycles(isrCallback, Period, scale, AUTO);
    }

    bool begin(void (*isrCallback)(), uint16_t Period, bool scale, TIMid id) {
        if (Period == 0 || Period > MAX_PERIOD)
            return false;

        return beginCycles(isrCallback, Period, scale, id);
    }

    void end();
    void interrupt_SIT(action ACT);
    void resetPeriod_SIT(uint16_t newPeriod, bool scale);
    int8_t isAllocated_SIT(void);

    static ISRcallback SIT_CALLBACK[NUM_SIT];
};


#ifdef __cplusplus
}
#endif

#endif
