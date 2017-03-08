#include "flash_map.h"
#include "rtc.h"
#include "sdkconfig.h"
#include "esp_image_format.h"
#include "rom/spi_flash.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/timer_group_reg.h"
#include "soc/dport_reg.h"
#include "soc/timer_group_struct.h"
#include "esp_intr_alloc.h"
#include "esp_intr.h"
#include "esp_flash_partitions.h"
#include "boot_debug.h"
#include "ui_hal.h"

/* #include "xtensa_intr_asm.S" */

/**
 * @brief  System Clock Configuration
 * @param  96M
 * @retval None
 */
void SystemClock_Config(void)
{
    rtc_set_cpu_freq(CPU_XTAL);
    rtc_set_cpu_freq(CPU_80M);
}
/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    BOOT_DEBUG("timer...\r\n");
    HAL_UI_SysTick_Handler();
}

#if 0
static void xt_ints(void)
{
/* xt_ints_on: */

    /* ENTRY0 */

asm volatile(
#if XCHAL_HAVE_INTERRUPTS
#if XT_USE_SWPRI
    movi    a3, 0
    movi    a4, _xt_intdata
    xsr     a3, INTENABLE        /* Disables all interrupts   */
    rsync
    l32i    a3, a4, 0            /* a3 = _xt_intenable        */
    l32i    a6, a4, 4            /* a6 = _xt_vpri_mask        */
    or      a5, a3, a2           /* a5 = _xt_intenable | mask */
    s32i    a5, a4, 0            /* _xt_intenable |= mask     */
    and     a5, a5, a6           /* a5 = _xt_intenable & _xt_vpri_mask */
    wsr     a5, INTENABLE        /* Reenable interrupts       */
    mov     a2, a3               /* Previous mask             */
#else
    movi    a3, 0
    xsr     a3, INTENABLE        /* Disables all interrupts   */
    rsync
    or      a2, a3, a2           /* set bits in mask */
    wsr     a2, INTENABLE        /* Re-enable ints */
    rsync
    mov     a2, a3               /* return prev mask */
#endif
#else
    movi    a2, 0                /* Return zero */
#endif
         )
    RET0
}
#endif

#if 0
typedef struct xt_handler_table_entry {
    void * handler;
    void * arg;
} xt_handler_table_entry;

#define XCHAL_NUM_INTERRUPTS		32	/* number of interrupts */
#define portNUM_PROCESSORS 1
extern xt_handler_table_entry _xt_interrupt_table[XCHAL_NUM_INTERRUPTS*portNUM_PROCESSORS];
//extern xt_handler_table_entry _xt_interrupt_table[32];


/*
  Default handler for unhandled interrupts.
*/
void xt_unhandled_interrupt(void * arg)
{
	ets_printf("Unhandled interrupt %d on cpu %d!\n", (int)arg, xPortGetCoreID());
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
#if 0
/**
 * @brief  System Clock Configuration
 * @param  96M
 * @retval None
 */
void HwTimer_config(void)
{
    BOOT_DEBUG("timer config \r\n");
    //use hardware timer0 init
    // enable periph timer0
    /* periph_module_enable(PERIPH_TIMG0_MODULE); */
    SET_PERI_REG_MASK(DPORT_PERIP_CLK_EN_REG, DPORT_TIMERGROUP_CLK_EN);
    CLEAR_PERI_REG_MASK(DPORT_PERIP_RST_EN_REG, DPORT_TIMERGROUP_RST);
    // init timer0
    /* TIMER_ENTER_CRITICAL(&timer_spinlock[0]); */
    TIMERG0.hw_timer[0].config.autoreload = 1;
    TIMERG0.hw_timer[0].config.divider = 80;
    TIMERG0.hw_timer[0].config.enable = 0;
    TIMERG0.hw_timer[0].config.increase = 1;
    TIMERG0.hw_timer[0].config.alarm_en = 0;
    TIMERG0.hw_timer[0].config.level_int_en = 1;
    TIMERG0.hw_timer[0].config.edge_int_en = 0;
    /* TIMER_EXIT_CRITICAL(&timer_spinlock[0]); */

    /*Stop timer counter*/
    /* TIMER_ENTER_CRITICAL(&timer_spinlock[0]); */
    /* TIMERG0.hw_timer[0].config.enable = 0; */
    /* TIMER_EXIT_CRITICAL(&timer_spinlock[0]); */

    /*Load counter value */
    /* TIMER_ENTER_CRITICAL(&timer_spinlock[0]); */
    TIMERG0.hw_timer[0].load_high = 0;//(uint32_t) (0 >> 32);
    TIMERG0.hw_timer[0].load_low = (uint32_t) 1000;
    TIMERG0.hw_timer[0].reload = 1;
    /* TIMER_EXIT_CRITICAL(&timer_spinlock[0]); */

    /*Enable timer interrupt*/
    /* timer_group_intr_enable(group_num, BIT(timer_num)); */
    /* portENTER_CRITICAL(&timer_spinlock[0]); */
    TIMERG0.int_ena.val |= 1;
    /* portEXIT_CRITICAL(&timer_spinlock[0]); */

    /*Set ISR handler*/
    /* timer_isr_register(timer_group, timer_idx, timer_group0_isr, (void*) timer_idx, ESP_INTR_FLAG_IRAM, NULL); */
    #if 0
    int intr_source = 0;
    uint32_t status_reg = 0;
    int mask = 0;
    if((ESP_INTR_FLAG_IRAM & ESP_INTR_FLAG_EDGE) == 0) {
        intr_source = ETS_TG0_T0_LEVEL_INTR_SOURCE + 0;
    } else {
        intr_source = ETS_TG0_T0_EDGE_INTR_SOURCE + 0;
    }
    status_reg = TIMG_INT_ST_TIMERS_REG(0);
    mask = 1<<0;
    #endif
    #if 0
asm volatile(
         .data
         .global _xt_interrupt_table
         .align  8

         _xt_interrupt_table:

         .set    i, 0
         .rept   XCHAL_NUM_INTERRUPTS*portNUM_PROCESSORS
         .word   xt_unhandled_interrupt      /* handler address               */
         .word   i                           /* handler arg (default: intnum) */
         .set    i, i+1
         .endr
        )
/* #endasm */
#endif
    /* esp_intr_alloc_intrstatus(intr_source, intr_alloc_flags, status_reg, mask, fn, arg, handle); */
    /* esp_intr_alloc_intrstatus(intr_source, ESP_INTR_FLAG_IRAM, status_reg, mask, SysTick_Handler, (void*)0, NULL); */

    /* xt_set_interrupt_handler(0, SysTick_Handler, NULL); */
    intr_matrix_set(xPortGetCoreID(),ETS_TG0_T0_LEVEL_INTR_SOURCE,10);
    /* intr_matrix_set(0,ETS_TG0_T0_LEVEL_INTR_SOURCE,10); */
    //Enable int at CPU-level;
    /* ESP_INTR_ENABLE(10); */
    /* xt_ints_on(10); */

    /* ets_isr_attach(10,SysTick_Handler,NULL); */
    /* ets_isr_mask(1<<10); */
/* ETS_TG0_T0_INTR_ATTACH(SysTick_Handler,NULL); */

    /*Start timer counter*/
    /* timer_start(timer_group, timer_idx); */
    /* TIMER_ENTER_CRITICAL(&timer_spinlock[0]); */
    TIMERG0.hw_timer[0].config.enable = 1;
    /* TIMER_EXIT_CRITICAL(&timer_spinlock[0]); */


}
#endif
static void uart_console_configure(void)
{
    const int uart_num = CONFIG_CONSOLE_UART_NUM;

    uartAttach();
    ets_install_uart_printf();

    // ROM bootloader may have put a lot of text into UART0 FIFO.
    // Wait for it to be printed.
    uart_tx_wait_idle(0);

    // Set configured UART console baud rate
    const int uart_baud = CONFIG_CONSOLE_UART_BAUDRATE;
    uart_div_modify(uart_num, (APB_CLK_FREQ << 4) / uart_baud);
}

/**
 * @brief  update flash config
 * @param  96M
 * @retval None
 */
static void Update_flash_config(void)
{
    esp_image_header_t fhdr;
    uint32_t size;

    if(esp_image_load_header(APP_ADDR, true, &fhdr) != ESP_OK) {
        return;
    }
    SPIUnlock();

    switch(fhdr.spi_size) {
        case ESP_IMAGE_FLASH_SIZE_1MB:
            size = 1;
            break;
        case ESP_IMAGE_FLASH_SIZE_2MB:
            size = 2;
            break;
        case ESP_IMAGE_FLASH_SIZE_4MB:
            size = 4;
            break;
        case ESP_IMAGE_FLASH_SIZE_8MB:
            size = 8;
            break;
        case ESP_IMAGE_FLASH_SIZE_16MB:
            size = 16;
            break;
        default:
            size = 2;
    }
    Cache_Read_Disable( 0 );
    // Set flash chip size
    SPIParamCfg(g_rom_flashchip.deviceId, size * 0x100000, 0x10000, 0x1000, 0x100, 0xffff);
    // TODO: set mode
    // TODO: set frequency
    Cache_Flush(0);
    Cache_Read_Enable( 0 );
}

static bool load_partition_table(void)
{
    const esp_partition_info_t *partitions;
    const int ESP_PARTITION_TABLE_DATA_LEN = 0xC00; /* length of actual data (signature is appended to this) */
    char *partition_usage;
    esp_err_t err;
    int num_partitions;

    partitions = bootloader_mmap(ESP_PARTITION_TABLE_ADDR, ESP_PARTITION_TABLE_DATA_LEN);
    if (!partitions) {
        return false;
    }

    BOOT_DEBUG("mapped partition table 0x%x at 0x%x\r\n", ESP_PARTITION_TABLE_ADDR, (intptr_t)partitions);

    err = esp_partition_table_basic_verify(partitions, true, &num_partitions);
    if (err != ESP_OK) {
        BOOT_DEBUG("Failed to verify partition table\r\n");
        return false;
    }
    BOOT_DEBUG("Partition Table:\r\n");
    BOOT_DEBUG("## Label            Usage          Type ST Offset   Length\r\n");

    for(int i = 0; i < num_partitions; i++) {
        const esp_partition_info_t *partition = &partitions[i];
        BOOT_DEBUG("load partition table entry 0x%x\r\n", (intptr_t)partition);
        BOOT_DEBUG("type=%x subtype=%x\r\n", partition->type, partition->subtype);
        partition_usage = "unknown";

        /* valid partition table */
        switch(partition->type) {
        case PART_TYPE_APP: /* app partition */
            switch(partition->subtype) {
            case PART_SUBTYPE_FACTORY: /* factory binary */
                partition_usage = "factory app";
                break;
            case PART_SUBTYPE_TEST: /* test binary */
                partition_usage = "test app";
                break;
            default:
                /* OTA binary */
                if ((partition->subtype & ~PART_SUBTYPE_OTA_MASK) == PART_SUBTYPE_OTA_FLAG) {
                    partition_usage = "OTA app";
                }
                else {
                    partition_usage = "Unknown app";
                }
                break;
            }
            break; /* PART_TYPE_APP */
        case PART_TYPE_DATA: /* data partition */
            switch(partition->subtype) {
            case PART_SUBTYPE_DATA_OTA: /* ota data */
                partition_usage = "OTA data";
                break;
            case PART_SUBTYPE_DATA_RF:
                partition_usage = "RF data";
                break;
            case PART_SUBTYPE_DATA_WIFI:
                partition_usage = "WiFi data";
                break;
            default:
                partition_usage = "Unknown data";
                break;
            }
            break; /* PARTITION_USAGE_DATA */
        default: /* other partition type */
            break;
        }

        /* print partition type info */
        BOOT_DEBUG("%2d %-16s %-16s %02x %02x %08x %08x\r\n", i, partition->label, partition_usage,
                 partition->type, partition->subtype,
                 partition->pos.offset, partition->pos.size);
    }

    bootloader_munmap(partitions);
    BOOT_DEBUG("End of partition table\r\n");
    return true;
}

/**
 * @brief  Configures Main system clocks & power.
 * @param  None
 * @retval None
 */
void Set_System(void)
{
    SystemClock_Config();
    uart_console_configure();
    /* disable watch dog here */
    REG_CLR_BIT( RTC_CNTL_WDTCONFIG0_REG, RTC_CNTL_WDT_FLASHBOOT_MOD_EN );
    REG_CLR_BIT( TIMG_WDTCONFIG0_REG(0), TIMG_WDT_FLASHBOOT_MOD_EN );
    bootloader_random_enable();
    Update_flash_config();
    load_partition_table();
    bootloader_random_disable();

    HAL_UI_Initial();
}

void delay(uint32_t ms)
{
    ets_delay_us(ms*1000);
}

void System_Reset(void)
{
    _ResetVector();
}

/* empty rtc_printf implementation, to work with librtc
   linking. Can be removed once -lrtc is removed from bootloader's
   main component.mk.
*/
int rtc_printf(void)
{
    return 0;
}
