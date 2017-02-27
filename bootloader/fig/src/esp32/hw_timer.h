#ifndef HW_TIMER_H_
#define HW_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
//#include "esp32-hal.h"

typedef struct {
    union {
        struct {
            uint32_t reserved0:   10;
            uint32_t alarm_en:     1;             /*When set  alarm is enabled*/
            uint32_t level_int_en: 1;             /*When set  level type interrupt will be generated during alarm*/
            uint32_t edge_int_en:  1;             /*When set  edge type interrupt will be generated during alarm*/
            uint32_t divider:     16;             /*Timer clock (T0/1_clk) pre-scale value.*/
            uint32_t autoreload:   1;             /*When set  timer 0/1 auto-reload at alarming is enabled*/
            uint32_t increase:     1;             /*When set  timer 0/1 time-base counter increment. When cleared timer 0 time-base counter decrement.*/
            uint32_t enable:       1;             /*When set  timer 0/1 time-base counter is enabled*/
        };
        uint32_t val;
    } config;
    uint32_t cnt_low;                             /*Register to store timer 0/1 time-base counter current value lower 32 bits.*/
    uint32_t cnt_high;                            /*Register to store timer 0 time-base counter current value higher 32 bits.*/
    uint32_t update;                              /*Write any value will trigger a timer 0 time-base counter value update (timer 0 current value will be stored in registers above)*/
    uint32_t alarm_low;                           /*Timer 0 time-base counter value lower 32 bits that will trigger the alarm*/
    uint32_t alarm_high;                          /*Timer 0 time-base counter value higher 32 bits that will trigger the alarm*/
    uint32_t load_low;                            /*Lower 32 bits of the value that will load into timer 0 time-base counter*/
    uint32_t load_high;                           /*higher 32 bits of the value that will load into timer 0 time-base counter*/
    uint32_t reload;                              /*Write any value will trigger timer 0 time-base counter reload*/
} hw_timer_reg_t;

typedef struct hw_timer_s {
        hw_timer_reg_t * dev;
        uint8_t num;
        uint8_t group;
        uint8_t timer;
} hw_timer_t;


hw_timer_t * timerBegin(uint8_t timer, uint16_t divider, bool countUp);
void timerEnd(hw_timer_t *timer);

void timerSetConfig(hw_timer_t *timer, uint32_t config);
uint32_t timerGetConfig(hw_timer_t *timer);

void timerAttachInterrupt(hw_timer_t *timer, void (*fn)(void), bool edge);
void timerDetachInterrupt(hw_timer_t *timer);

void timerStart(hw_timer_t *timer);
void timerStop(hw_timer_t *timer);
void timerRestart(hw_timer_t *timer);
void timerWrite(hw_timer_t *timer, uint64_t val);
void timerSetDivider(hw_timer_t *timer, uint16_t divider);
void timerSetCountUp(hw_timer_t *timer, bool countUp);
void timerSetAutoReload(hw_timer_t *timer, bool autoreload);

bool timerStarted(hw_timer_t *timer);
uint64_t timerRead(hw_timer_t *timer);
uint64_t timerReadMicros(hw_timer_t *timer);
double timerReadSeconds(hw_timer_t *timer);
uint16_t timerGetDivider(hw_timer_t *timer);
bool timerGetCountUp(hw_timer_t *timer);
bool timerGetAutoReload(hw_timer_t *timer);

void timerAlarmEnable(hw_timer_t *timer);
void timerAlarmDisable(hw_timer_t *timer);
void timerAlarmWrite(hw_timer_t *timer, uint64_t interruptAt, bool autoreload);

bool timerAlarmEnabled(hw_timer_t *timer);
uint64_t timerAlarmRead(hw_timer_t *timer);
uint64_t timerAlarmReadMicros(hw_timer_t *timer);
double timerAlarmReadSeconds(hw_timer_t *timer);


#ifdef __cplusplus
}
#endif

#endif /* MAIN_ESP32_HAL_TIMER_H_ */
