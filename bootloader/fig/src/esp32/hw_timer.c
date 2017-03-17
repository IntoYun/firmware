#include "hw_timer.h"
#include "rom/ets_sys.h"
#include "soc/timer_group_struct.h"
#include "soc/dport_reg.h"
#include "esp_attr.h"
#include "esp_intr.h"

#define HWTIMER_INUM 10


static hw_timer_t hw_timer[4] = {
        {(hw_timer_reg_t *)(DR_REG_TIMERGROUP0_BASE),0,0,0},
        {(hw_timer_reg_t *)(DR_REG_TIMERGROUP0_BASE + 0x0024),1,0,1},
        {(hw_timer_reg_t *)(DR_REG_TIMERGROUP0_BASE + 0x1000),2,1,0},
        {(hw_timer_reg_t *)(DR_REG_TIMERGROUP0_BASE + 0x1024),3,1,1}
};

typedef void (*voidFuncPtr)(void);
static voidFuncPtr __timerInterruptHandlers[4] = {0,};

#if 0
typedef struct xt_handler_table_entry {
    void * handler;
    void * arg;
} xt_handler_table_entry;

#define XCHAL_NUM_INTERRUPTS		32	/* number of interrupts */
#define portNUM_PROCESSORS 1

extern xt_handler_table_entry _xt_interrupt_table[XCHAL_NUM_INTERRUPTS*portNUM_PROCESSORS];

/*
  Default handler for unhandled interrupts.
*/
void xt_unhandled_interrupt(void * arg)
{
	ets_printf("Unhandled interrupt %d on cpu %d!\n", (int)arg, 0);
}
/*
static inline uint32_t xPortGetCoreID() {
    int id;
    asm volatile(
                 "rsr.prid %0\n"
                 " extui %0,%0,13,1"
                 :"=r"(id));
    return id;
}
*/

/*
  This function registers a handler for the specified interrupt. The "arg"
  parameter specifies the argument to be passed to the handler when it is
  invoked. The function returns the address of the previous handler.
  On error, it returns 0.
*/
xt_handler xt_set_interrupt_handler(int n, xt_handler f, void * arg)
{
    xt_handler_table_entry * entry;
    xt_handler               old;

    if( n < 0 || n >= XCHAL_NUM_INTERRUPTS )
        return 0;       /* invalid interrupt number */
    if( Xthal_intlevel[n] > XCHAL_EXCM_LEVEL )
        return 0;       /* priority level too high to safely handle in C */

    /* Convert exception number to _xt_exception_table name */
    //n = n * portNUM_PROCESSORS + xPortGetCoreID();
    n = n * portNUM_PROCESSORS + 0;

    entry = _xt_interrupt_table + n;
    old   = entry->handler;

    if (f) {
        entry->handler = f;
        entry->arg     = arg;
    }
    else {
        entry->handler = &xt_unhandled_interrupt;
        entry->arg     = (void*)n;
    }

    return ((old == &xt_unhandled_interrupt) ? 0 : old);
}
#endif

void IRAM_ATTR __timerISR(void * arg){
    uint32_t s0 = TIMERG0.int_st_timers.val;
    uint32_t s1 = TIMERG1.int_st_timers.val;
    TIMERG0.int_clr_timers.val = s0;
    TIMERG1.int_clr_timers.val = s1;
    uint8_t status = (s1 & 3) << 2 | (s0 & 3);
    uint8_t i = 4;
    //restart the timers that should autoreload
    while(i--){
        hw_timer_reg_t * dev = hw_timer[i].dev;
        if((status & (1 << i)) && dev->config.autoreload){
            dev->load_high = 0;
            dev->load_low = 0;
            dev->reload = 1;
            dev->config.alarm_en = 1;
        }
    }
    i = 4;
    //call callbacks
    while(i--){
        if(status & (1 << i)){
            if(__timerInterruptHandlers[i]){
                __timerInterruptHandlers[i]();
            }
        }
    }
}

uint64_t timerRead(hw_timer_t *timer){
    timer->dev->update = 1;
    uint64_t h = timer->dev->cnt_high;
    uint64_t l = timer->dev->cnt_low;
    return (h << 32) | l;
}

uint64_t timerAlarmRead(hw_timer_t *timer){
    uint64_t h = timer->dev->alarm_high;
    uint64_t l = timer->dev->alarm_low;
    return (h << 32) | l;
}

void timerWrite(hw_timer_t *timer, uint64_t val){
    timer->dev->load_high = (uint32_t) (val >> 32);
    timer->dev->load_low = (uint32_t) (val);
    timer->dev->reload = 1;
}

void timerAlarmWrite(hw_timer_t *timer, uint64_t alarm_value, bool autoreload){
    timer->dev->alarm_high = (uint32_t) (alarm_value >> 32);
    timer->dev->alarm_low = (uint32_t) alarm_value;
    timer->dev->config.autoreload = autoreload;
}

void timerSetConfig(hw_timer_t *timer, uint32_t config){
    timer->dev->config.val = config;
}

uint32_t timerGetConfig(hw_timer_t *timer){
    return timer->dev->config.val;
}

void timerSetCountUp(hw_timer_t *timer, bool countUp){
    timer->dev->config.increase = countUp;
}

bool timerGetCountUp(hw_timer_t *timer){
    return timer->dev->config.increase;
}

void timerSetAutoReload(hw_timer_t *timer, bool autoreload){
    timer->dev->config.autoreload = autoreload;
}

bool timerGetAutoReload(hw_timer_t *timer){
    return timer->dev->config.autoreload;
}

void timerSetDivider(hw_timer_t *timer, uint16_t divider){//2 to 65536
    if(!divider){
        divider = 0xFFFF;
    } else if(divider == 1){
        divider = 2;
    }
    int timer_en = timer->dev->config.enable;
    timer->dev->config.enable = 0;
    timer->dev->config.divider = divider;
    timer->dev->config.enable = timer_en;
}

uint16_t timerGetDivider(hw_timer_t *timer){
    return timer->dev->config.divider;
}

void timerStart(hw_timer_t *timer){
    timer->dev->config.enable = 1;
}

void timerStop(hw_timer_t *timer){
    timer->dev->config.enable = 0;
}

void timerRestart(hw_timer_t *timer){
    timer->dev->config.enable = 0;
    timer->dev->config.enable = 1;
}

bool timerStarted(hw_timer_t *timer){
    return timer->dev->config.enable;
}

void timerAlarmEnable(hw_timer_t *timer){
    timer->dev->config.alarm_en = 1;
}

void timerAlarmDisable(hw_timer_t *timer){
    timer->dev->config.alarm_en = 0;
}

bool timerAlarmEnabled(hw_timer_t *timer){
    return timer->dev->config.alarm_en;
}

hw_timer_t * timerBegin(uint8_t num, uint16_t divider, bool countUp){
    if(num > 3){
        return NULL;
    }
    hw_timer_t * timer = &hw_timer[num];
    if(timer->group) {
        SET_PERI_REG_MASK(DPORT_PERIP_CLK_EN_REG, DPORT_TIMERGROUP1_CLK_EN);
        CLEAR_PERI_REG_MASK(DPORT_PERIP_RST_EN_REG, DPORT_TIMERGROUP1_RST);
        TIMERG1.int_ena.val &= ~BIT(timer->timer);
    } else {
        SET_PERI_REG_MASK(DPORT_PERIP_CLK_EN_REG, DPORT_TIMERGROUP_CLK_EN);
        CLEAR_PERI_REG_MASK(DPORT_PERIP_RST_EN_REG, DPORT_TIMERGROUP_RST);
        TIMERG0.int_ena.val &= ~BIT(timer->timer);
    }
    timer->dev->config.enable = 0;
    timerSetDivider(timer, divider);
    timerSetCountUp(timer, countUp);
    timerSetAutoReload(timer, false);
    timerAttachInterrupt(timer, NULL, false);
    timerWrite(timer, 0);
    timer->dev->config.enable = 1;
    return timer;
}

void timerEnd(hw_timer_t *timer){
    timer->dev->config.enable = 0;
    timerAttachInterrupt(timer, NULL, false);
}

void timerAttachInterrupt(hw_timer_t *timer, void (*fn)(void), bool edge){
    static bool initialized = false;
    //ESP_INTR_DISABLE(HWTIMER_INUM);
    if(fn == NULL){
        timer->dev->config.level_int_en = 0;
        timer->dev->config.edge_int_en = 0;
        timer->dev->config.alarm_en = 0;
        if(timer->num & 2){
            TIMERG1.int_ena.val &= ~BIT(timer->timer);
        } else {
            TIMERG0.int_ena.val &= ~BIT(timer->timer);
        }
        __timerInterruptHandlers[timer->num] = NULL;
    } else {
        if(!initialized){
            //xt_set_interrupt_handler(HWTIMER_INUM, &__timerISR, NULL);
            initialized = true;
        }
        __timerInterruptHandlers[timer->num] = fn;
        timer->dev->config.level_int_en = edge?0:1;//When set, an alarm will generate a level type interrupt.
        timer->dev->config.edge_int_en = edge?1:0;//When set, an alarm will generate an edge type interrupt.
        int intr_source = 0;
        if(!edge){
            if(timer->group){
                intr_source = ETS_TG1_T0_LEVEL_INTR_SOURCE + timer->timer;
            } else {
                intr_source = ETS_TG0_T0_LEVEL_INTR_SOURCE + timer->timer;
            }
        } else {
            if(timer->group){
                intr_source = ETS_TG1_T0_EDGE_INTR_SOURCE + timer->timer;
            } else {
                intr_source = ETS_TG0_T0_EDGE_INTR_SOURCE + timer->timer;
            }
        }
        intr_matrix_set(0, intr_source, HWTIMER_INUM);
        if(timer->group){
            TIMERG1.int_ena.val |= BIT(timer->timer);
        } else {
            TIMERG0.int_ena.val |= BIT(timer->timer);
        }
    }
    //ESP_INTR_ENABLE(HWTIMER_INUM);
}

void timerDetachInterrupt(hw_timer_t *timer){
    timerAttachInterrupt(timer, NULL, false);
}

uint64_t timerReadMicros(hw_timer_t *timer){
    uint64_t timer_val = timerRead(timer);
    uint16_t div = timerGetDivider(timer);
    return timer_val * div / 80;
}

double timerReadSeconds(hw_timer_t *timer){
    uint64_t timer_val = timerRead(timer);
    uint16_t div = timerGetDivider(timer);
    return (double)timer_val * div / 80000000;
}

uint64_t timerAlarmReadMicros(hw_timer_t *timer){
    uint64_t timer_val = timerAlarmRead(timer);
    uint16_t div = timerGetDivider(timer);
    return timer_val * div / 80;
}

double timerAlarmReadSeconds(hw_timer_t *timer){
    uint64_t timer_val = timerAlarmRead(timer);
    uint16_t div = timerGetDivider(timer);
    return (double)timer_val * div / 80000000;
}
