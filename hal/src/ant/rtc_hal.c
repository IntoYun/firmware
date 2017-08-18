/**
 ******************************************************************************
 * @file    rtc_hal.c
 * @author  Satish Nair, Brett Walach
 * @version V1.0.0
 * @date    12-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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
#include "hw_config.h"
#include "rtc_hal.h"
#include "gpio_hal.h"
#include <math.h>
#include "rtc_hal_lora.h"
#include "service_debug.h"
#include "interrupts_hal.h"
#include "ui_hal.h"

RTC_HandleTypeDef RtcHandle;
static bool rtcFailFlag = true;

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    RCC_OscInitTypeDef        RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    //__HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        rtcFailFlag = false;
        return;
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        rtcFailFlag = false;
        return;
    }
    __HAL_RCC_RTC_ENABLE();
}

bool GetRTCSatus(void)
{
    return rtcFailFlag;
}
/**
 * @brief RTC MSP De-Initialization
 *        This function frees the hardware resources
 *          - Disable the Peripheral's clock
 * @param hrtc: RTC handle pointer
 * @retval None
 */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
    __HAL_RCC_RTC_DISABLE();
    HAL_PWR_DisableBkUpAccess();
    __HAL_RCC_PWR_CLK_DISABLE();
}

time_t HAL_RTC_Get_UnixTime(void)
{
    #if 0
    RTC_DateTypeDef sdatestructureget;
    RTC_TimeTypeDef stimestructureget;
    struct tm tmstruct;
    memset(&tmstruct, 0, sizeof(struct tm));

    /* Get the RTC current Time */
    HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
    /* Get the RTC current Date */
    HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
    tmstruct.tm_year = 2000 + sdatestructureget.Year - 1900;
    tmstruct.tm_mon  = sdatestructureget.Month - 1;
    tmstruct.tm_mday = sdatestructureget.Date;
    tmstruct.tm_hour = stimestructureget.Hours;
    tmstruct.tm_min  = stimestructureget.Minutes;
    tmstruct.tm_sec  = stimestructureget.Seconds;
    time_t t = mktime( &tmstruct );
    return t;
    #endif
    return 0;
}

/*
 * @brief Direct transform dec data to hex data, for the Set_UnixTime function.
 *        This function not convert the data. Example: dec:16 -> hex:0x16
 * @param decData: The input decimal data
 * @retral The output hexadecimal data
 */
static int dec2hex_direct(uint8_t decData)
{
    int hexData  = 0;
    uint8_t iCount   = 0;
    while( (decData / 10) || (decData % 10) ){
        hexData = hexData +  (decData % 10) * pow(16, iCount);
        if (decData < 10) {
            break;
        }
        decData = decData / 10;
        iCount++;
    }
    return hexData;
}

void HAL_RTC_Set_UnixTime(time_t value)
{
    #if 0
    struct tm *tmTemp = gmtime( &value );
    RTC_DateTypeDef sdatestructure;
    RTC_TimeTypeDef stimestructure;

    /*##-1- Configure the Date #################################################*/
    /* Set Date: Friday January 1st 2016 */
    sdatestructure.Year    = dec2hex_direct(tmTemp->tm_year + 1900 -2000);
    sdatestructure.Month   = dec2hex_direct(tmTemp->tm_mon + 1);
    sdatestructure.Date    = dec2hex_direct(tmTemp->tm_mday);
    sdatestructure.WeekDay = RTC_WEEKDAY_FRIDAY;

    if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
    {
        DEBUG("RTC Set_UnixTime SetDate failed!");
    }

    /*##-2- Configure the Time #################################################*/
    /* Set Time: 00:00:00 */
    stimestructure.Hours          = dec2hex_direct(tmTemp->tm_hour);
    stimestructure.Minutes        = dec2hex_direct(tmTemp->tm_min);
    stimestructure.Seconds        = dec2hex_direct(tmTemp->tm_sec);
    stimestructure.TimeFormat     = RTC_HOURFORMAT12_AM;
    stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
    stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

    if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
    {
        DEBUG("RTC Set_UnixTime SetTime failed!");
    }
    #endif
}

/*!
 * RTC Time base in ms
 */
#define RTC_ALARM_TICK_DURATION                     0.48828125      // 1 tick every 488us
#define RTC_ALARM_TICK_PER_MS                       2.048           // 1/2.048 = tick duration in ms

/*!
 * Maximum number of days that can be handled by the RTC alarm counter before overflow.
 */
#define RTC_ALARM_MAX_NUMBER_OF_DAYS                28

/*!
 * Number of seconds in a minute
 */
static const uint8_t SecondsInMinute = 60;

/*!
 * Number of seconds in an hour
 */
static const uint16_t SecondsInHour = 3600;

/*!
 * Number of seconds in a day
 */
static const uint32_t SecondsInDay = 86400;

/*!
 * Number of hours in a day
 */
static const uint8_t HoursInDay = 24;

/*!
 * Number of seconds in a leap year
 */
static const uint32_t SecondsInLeapYear = 31622400;

/*!
 * Number of seconds in a year
 */
static const uint32_t SecondsInYear = 31536000;

/*!
 * Number of days in each month on a normal year
 */
static const uint8_t DaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*!
 * Number of days in each month on a leap year
 */
static const uint8_t DaysInMonthLeapYear[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*!
 * Holds the current century for real time computation
 */
static uint16_t Century = 0;

/*!
 * Flag used to indicates a Calendar Roll Over is about to happen
 */
static bool CalendarRollOverReady = false;

/*!
 * Flag used to indicates a the MCU has waken-up from an external IRQ
 */
volatile bool NonScheduledWakeUp = false;

/*!
 * RTC timer context
 */
typedef struct RtcCalendar_s
{
    uint16_t CalendarCentury;     //! Keep track of century value
    RTC_DateTypeDef CalendarDate; //! Reference time in calendar format
    RTC_TimeTypeDef CalendarTime; //! Reference date in calendar format
} RtcCalendar_t;

/*!
 * Current RTC timer context
 */
RtcCalendar_t RtcCalendarContext;

/*!
 * \brief Flag to indicate if the timestamps until the next event is long enough
 * to set the MCU into low power mode
 */
static bool RtcTimerEventAllowsLowPower = false;

/*!
 * \brief Flag to disable the LowPower Mode even if the timestamps until the
 * next event is long enough to allow Low Power mode
 */
static bool LowPowerDisableDuringTask = false;

/*!
 * \brief RTC Handler
 */
/* RTC_HandleTypeDef RtcHandle = { 0 }; */

/*!
 * \brief Indicates if the RTC is already Initialized or not
 */
static bool RtcInitialized = false;

/*!
 * \brief Indicates if the RTC Wake Up Time is calibrated or not
 */
static bool WakeUpTimeInitialized = false;

/*!
 * \brief Hold the Wake-up time duration in ms
 */
volatile uint32_t McuWakeUpTime = 0;

/*!
 * \brief Hold the cumulated error in micro-second to compensate the timing errors
 */
static int32_t TimeoutValueError = 0;

/*!
 * \brief RTC wakeup time computation
 */
static void RtcComputeWakeUpTime( void );

/*!
 * \brief Start the RTC Alarm (timeoutValue is in ms)
 */
static void RtcStartWakeUpAlarm( uint32_t timeoutValue );

/*!
 * \brief Converts a TimerTime_t value into RtcCalendar_t value
 *
 * \param[IN] timeCounter Value to convert to RTC calendar
 * \retval rtcCalendar New RTC calendar value
 */
//
// REMARK: Removed function static attribute in order to suppress
//         "#177-D function was declared but never referenced" warning.
// static RtcCalendar_t RtcConvertTimerTimeToCalendarTick( TimerTime_t timeCounter )
//
RtcCalendar_t RtcConvertTimerTimeToCalendarTick( TimerTime_t timeCounter );

/*!
 * \brief Converts a RtcCalendar_t value into TimerTime_t value
 *
 * \param[IN/OUT] calendar Calendar value to be converted
 *                         [NULL: compute from "now",
 *                          Others: compute from given calendar value]
 * \retval timerTime New TimerTime_t value
 */
static TimerTime_t RtcConvertCalendarTickToTimerTime( RtcCalendar_t *calendar );

/*!
 * \brief Converts a TimerTime_t value into a value for the RTC Alarm
 *
 * \param[IN] timeCounter Value in ms to convert into a calendar alarm date
 * \param[IN] now Current RTC calendar context
 * \retval rtcCalendar Value for the RTC Alarm
 */
static RtcCalendar_t RtcComputeTimerTimeToAlarmTick( TimerTime_t timeCounter, RtcCalendar_t now );

/*!
 * \brief Returns the internal RTC Calendar and check for RTC overflow
 *
 * \retval calendar RTC calendar
 */
static RtcCalendar_t RtcGetCalendar( void );

/*!
 * \brief Check the status for the calendar year to increase the value of Century at the overflow of the RTC
 *
 * \param[IN] year Calendar current year
 */
static void RtcCheckCalendarRollOver( uint8_t year );



/* void RtcInit( void ) */
void HAL_RTC_Initial( void )
{
    RtcCalendar_t rtcInit;

    if( RtcInitialized == false )
    {
        __HAL_RCC_RTC_ENABLE( );

        RtcHandle.Instance = RTC;
        RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
        //设置32.768K的分频系数
        //Fclk = 32768/[(SynchPrediv+1)*(AsynchPrediv+1)]
        RtcHandle.Init.AsynchPrediv = 3;
        RtcHandle.Init.SynchPrediv = 3;

        //set rtc time base to 1s
        /* RtcHandle.Init.AsynchPrediv = 0x7f; */
        /* RtcHandle.Init.SynchPrediv = 0xff; */


        RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
        RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
        RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
        HAL_RTC_Init( &RtcHandle );

        // Set Date: Friday 1st of January 2000
        rtcInit.CalendarDate.Year = 0;
        rtcInit.CalendarDate.Month = 1;
        rtcInit.CalendarDate.Date = 1;
        rtcInit.CalendarDate.WeekDay = RTC_WEEKDAY_SATURDAY;
        HAL_RTC_SetDate( &RtcHandle, &rtcInit.CalendarDate, RTC_FORMAT_BIN );

        // Set Time: 00:00:00
        rtcInit.CalendarTime.Hours = 0;
        rtcInit.CalendarTime.Minutes = 0;
        rtcInit.CalendarTime.Seconds = 0;
        rtcInit.CalendarTime.TimeFormat = RTC_HOURFORMAT12_AM;
        rtcInit.CalendarTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        rtcInit.CalendarTime.StoreOperation = RTC_STOREOPERATION_RESET;
        HAL_RTC_SetTime( &RtcHandle, &rtcInit.CalendarTime, RTC_FORMAT_BIN );

        HAL_NVIC_SetPriority( RTC_Alarm_IRQn, 4, 0 );
        HAL_NVIC_EnableIRQ( RTC_Alarm_IRQn );
        RtcInitialized = true;
    }
}

void RtcSetTimeout( uint32_t timeout )
{
    RtcStartWakeUpAlarm( timeout );
}

TimerTime_t RtcGetAdjustedTimeoutValue( uint32_t timeout )
{
    if( timeout > McuWakeUpTime )
    {   // we have waken up from a GPIO and we have lost "McuWakeUpTime" that we need to compensate on next event
        if( NonScheduledWakeUp == true )
        {
            NonScheduledWakeUp = false;
            timeout -= McuWakeUpTime;
        }
    }

    if( timeout > McuWakeUpTime )
    {   // we don't go in Low Power mode for delay below 50ms (needed for LEDs)
        if( timeout < 50 ) // 50 ms
        {
            RtcTimerEventAllowsLowPower = false;
        }
        else
        {
            RtcTimerEventAllowsLowPower = true;
            timeout -= McuWakeUpTime;
        }
    }
    return  timeout;
}

TimerTime_t RtcGetTimerValue( void )
{
    return( RtcConvertCalendarTickToTimerTime( NULL ) );
}

TimerTime_t RtcGetElapsedAlarmTime( void )
{
    TimerTime_t currentTime = 0;
    TimerTime_t contextTime = 0;

    currentTime = RtcConvertCalendarTickToTimerTime( NULL );
    contextTime = RtcConvertCalendarTickToTimerTime( &RtcCalendarContext );

    if( currentTime < contextTime )
    {
        return( currentTime + ( 0xFFFFFFFF - contextTime ) );
    }
    else
    {
        return( currentTime - contextTime );
    }
}

TimerTime_t RtcComputeFutureEventTime( TimerTime_t futureEventInTime )
{
    return( RtcGetTimerValue( ) + futureEventInTime );
}

TimerTime_t RtcComputeElapsedTime( TimerTime_t eventInTime )
{
    TimerTime_t elapsedTime = 0;

    // Needed at boot, cannot compute with 0 or elapsed time will be equal to current time
    if( eventInTime == 0 )
    {
        return 0;
    }

    elapsedTime = RtcConvertCalendarTickToTimerTime( NULL );

    if( elapsedTime < eventInTime )
    { // roll over of the counter
        return( elapsedTime + ( 0xFFFFFFFF - eventInTime ) );
    }
    else
    {
        return( elapsedTime - eventInTime );
    }
}

void BlockLowPowerDuringTask ( bool status )
{
    if( status == true )
    {
        RtcRecoverMcuStatus( );
    }
    LowPowerDisableDuringTask = status;
}

void RtcEnterLowPowerStopMode( void )
{
    if( ( LowPowerDisableDuringTask == false ) && ( RtcTimerEventAllowsLowPower == true ) )
    {
        BoardDeInitMcu( );

        // Disable the Power Voltage Detector
        HAL_PWR_DisablePVD( );

        SET_BIT( PWR->CR, PWR_CR_CWUF );

        // Enable Ultra low power mode
        HAL_PWREx_EnableUltraLowPower( );

        // Enable the fast wake up from Ultra low power mode
        HAL_PWREx_EnableFastWakeUp( );

        // Enter Stop Mode
        HAL_PWR_EnterSTOPMode( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );
    }
}

void RtcRecoverMcuStatus( void )
{
    // PWR_FLAG_WU indicates the Alarm has waken-up the MCU
    if( __HAL_PWR_GET_FLAG( PWR_FLAG_WU ) != RESET )
    {
        __HAL_PWR_CLEAR_FLAG( PWR_FLAG_WU );
    }
    else
    {
        NonScheduledWakeUp = true;
    }
    // check the clk source and set to full speed if we are coming from sleep mode
    if( ( __HAL_RCC_GET_SYSCLK_SOURCE( ) == RCC_SYSCLKSOURCE_STATUS_HSI ) ||
        ( __HAL_RCC_GET_SYSCLK_SOURCE( ) == RCC_SYSCLKSOURCE_STATUS_MSI ) )
    {
        BoardInitMcu( );
    }
}

static void RtcComputeWakeUpTime( void )
{
    uint32_t start = 0;
    uint32_t stop = 0;
    RTC_AlarmTypeDef  alarmRtc;
    RtcCalendar_t now;

    if( WakeUpTimeInitialized == false )
    {
        now = RtcGetCalendar( );
        HAL_RTC_GetAlarm( &RtcHandle, &alarmRtc, RTC_ALARM_A, RTC_FORMAT_BIN );

        start = alarmRtc.AlarmTime.Seconds + ( SecondsInMinute * alarmRtc.AlarmTime.Minutes ) + ( SecondsInHour * alarmRtc.AlarmTime.Hours );
        stop = now.CalendarTime.Seconds + ( SecondsInMinute * now.CalendarTime.Minutes ) + ( SecondsInHour * now.CalendarTime.Hours );

        McuWakeUpTime = ceil ( ( stop - start ) * RTC_ALARM_TICK_DURATION );

        WakeUpTimeInitialized = true;
    }
}

static void RtcStartWakeUpAlarm( uint32_t timeoutValue )
{
    RtcCalendar_t now;
    RtcCalendar_t alarmTimer;
    RTC_AlarmTypeDef alarmStructure;

    HAL_RTC_DeactivateAlarm( &RtcHandle, RTC_ALARM_A );
    HAL_RTCEx_DeactivateWakeUpTimer( &RtcHandle );

    // Load the RTC calendar
    now = RtcGetCalendar( );

    // Save the calendar into RtcCalendarContext to be able to calculate the elapsed time
    RtcCalendarContext = now;

    // timeoutValue is in ms
    alarmTimer = RtcComputeTimerTimeToAlarmTick( timeoutValue, now );

    alarmStructure.Alarm = RTC_ALARM_A;
    alarmStructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    alarmStructure.AlarmMask = RTC_ALARMMASK_NONE;
    alarmStructure.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;

    alarmStructure.AlarmTime.Seconds = alarmTimer.CalendarTime.Seconds;
    alarmStructure.AlarmTime.Minutes = alarmTimer.CalendarTime.Minutes;
    alarmStructure.AlarmTime.Hours = alarmTimer.CalendarTime.Hours;
    alarmStructure.AlarmDateWeekDay = alarmTimer.CalendarDate.Date;

    if( HAL_RTC_SetAlarm_IT( &RtcHandle, &alarmStructure, RTC_FORMAT_BIN ) != HAL_OK )
    {
        assert_param( FAIL );
    }
}

static RtcCalendar_t RtcComputeTimerTimeToAlarmTick( TimerTime_t timeCounter, RtcCalendar_t now )
{
    RtcCalendar_t calendar = now;

    uint16_t seconds = now.CalendarTime.Seconds;
    uint16_t minutes = now.CalendarTime.Minutes;
    uint16_t hours = now.CalendarTime.Hours;
    uint16_t days = now.CalendarDate.Date;
    double timeoutValueTemp = 0.0;
    double timeoutValue = 0.0;
    double error = 0.0;

    timeCounter = MIN( timeCounter, ( TimerTime_t )( RTC_ALARM_MAX_NUMBER_OF_DAYS * SecondsInDay * RTC_ALARM_TICK_DURATION ) );

    if( timeCounter < 1 )
    {
        timeCounter = 1;
    }

    // timeoutValue is used for complete computation
    timeoutValue = round( timeCounter * RTC_ALARM_TICK_PER_MS );

    // timeoutValueTemp is used to compensate the cumulating errors in timing far in the future
    timeoutValueTemp =  ( double )timeCounter * RTC_ALARM_TICK_PER_MS;

    // Compute timeoutValue error
    error = timeoutValue - timeoutValueTemp;

    // Add new error value to the cumulated value in uS
    TimeoutValueError += ( error  * 1000 );

    // Correct cumulated error if greater than ( RTC_ALARM_TICK_DURATION * 1000 )
    if( TimeoutValueError >= ( int32_t )( RTC_ALARM_TICK_DURATION * 1000 ) )
    {
        TimeoutValueError = TimeoutValueError - ( uint32_t )( RTC_ALARM_TICK_DURATION * 1000 );
        timeoutValue = timeoutValue + 1;
    }

    // Convert milliseconds to RTC format and add to now
    while( timeoutValue >= SecondsInDay )
    {
        timeoutValue -= SecondsInDay;
        days++;
    }

    // Calculate hours
    while( timeoutValue >= SecondsInHour )
    {
        timeoutValue -= SecondsInHour;
        hours++;
    }

    // Calculate minutes
    while( timeoutValue >= SecondsInMinute )
    {
        timeoutValue -= SecondsInMinute;
        minutes++;
    }

    // Calculate seconds
    seconds += timeoutValue;

    // Correct for modulo
    while( seconds >= 60 )
    {
        seconds -= 60;
        minutes++;
    }

    while( minutes >= 60 )
    {
        minutes -= 60;
        hours++;
    }

    while( hours >= HoursInDay )
    {
        hours -= HoursInDay;
        days++;
    }

    if( ( now.CalendarDate.Year == 0 ) || ( ( now.CalendarDate.Year + Century ) % 4 ) == 0 )
    {
        if( days > DaysInMonthLeapYear[now.CalendarDate.Month - 1] )
        {
            days = days % DaysInMonthLeapYear[now.CalendarDate.Month - 1];
        }
    }
    else
    {
        if( days > DaysInMonth[now.CalendarDate.Month - 1] )
        {
            days = days % DaysInMonth[now.CalendarDate.Month - 1];
        }
    }

    calendar.CalendarTime.Seconds = seconds;
    calendar.CalendarTime.Minutes = minutes;
    calendar.CalendarTime.Hours = hours;
    calendar.CalendarDate.Date = days;

    return calendar;
}

//
// REMARK: Removed function static attribute in order to suppress
//         "#177-D function was declared but never referenced" warning.
// static RtcCalendar_t RtcConvertTimerTimeToCalendarTick( TimerTime_t timeCounter )
//
RtcCalendar_t RtcConvertTimerTimeToCalendarTick( TimerTime_t timeCounter )
{
    RtcCalendar_t calendar = { 0 };

    uint16_t seconds = 0;
    uint16_t minutes = 0;
    uint16_t hours = 0;
    uint16_t days = 0;
    uint8_t months = 1; // Start at 1, month 0 does not exist
    uint16_t years = 0;
    uint16_t century = 0;
    double timeCounterTemp = 0.0;

    timeCounterTemp = ( double )timeCounter * RTC_ALARM_TICK_PER_MS;

    // Convert milliseconds to RTC format and add to now
    while( timeCounterTemp >= SecondsInLeapYear )
    {
        if( ( years == 0 ) || ( years % 4 ) == 0 )
        {
            timeCounterTemp -= SecondsInLeapYear;
        }
        else
        {
            timeCounterTemp -= SecondsInYear;
        }
        years++;
        if( years == 100 )
        {
            century = century + 100;
            years = 0;
        }
    }

    if( timeCounterTemp >= SecondsInYear )
    {
        if( ( years == 0 ) || ( years % 4 ) == 0 )
        {
            // Nothing to be done
        }
        else
        {
            timeCounterTemp -= SecondsInYear;
            years++;
        }
    }

    if( ( years == 0 ) || ( years % 4 ) == 0 )
    {
        while( timeCounterTemp >= ( DaysInMonthLeapYear[ months - 1 ] * SecondsInDay ) )
        {
            timeCounterTemp -= DaysInMonthLeapYear[ months - 1 ] * SecondsInDay;
            months++;
        }
    }
    else
    {
        while( timeCounterTemp >= ( DaysInMonth[ months - 1 ] * SecondsInDay ) )
        {
            timeCounterTemp -= DaysInMonth[ months - 1 ] * SecondsInDay;
            months++;
        }
    }

    // Convert milliseconds to RTC format and add to now
    while( timeCounterTemp >= SecondsInDay )
    {
        timeCounterTemp -= SecondsInDay;
        days++;
    }

    // Calculate hours
    while( timeCounterTemp >= SecondsInHour )
    {
        timeCounterTemp -= SecondsInHour;
        hours++;
    }

    // Calculate minutes
    while( timeCounterTemp >= SecondsInMinute )
    {
        timeCounterTemp -= SecondsInMinute;
        minutes++;
    }

    // Calculate seconds
    seconds = round( timeCounterTemp );

    calendar.CalendarTime.Seconds = seconds;
    calendar.CalendarTime.Minutes = minutes;
    calendar.CalendarTime.Hours = hours;
    calendar.CalendarDate.Date = days;
    calendar.CalendarDate.Month = months;
    calendar.CalendarDate.Year = years;
    calendar.CalendarCentury = century;

    return calendar;
}

static TimerTime_t RtcConvertCalendarTickToTimerTime( RtcCalendar_t *calendar )
{
    TimerTime_t timeCounter = 0;
    RtcCalendar_t now;
    double timeCounterTemp = 0.0;

    // Passing a NULL pointer will compute from "now" else,
    // compute from the given calendar value
    if( calendar == NULL )
    {
        now = RtcGetCalendar( );
    }
    else
    {
        now = *calendar;
    }

    // Years (calculation valid up to year 2099)
    for( int16_t i = 0; i < ( now.CalendarDate.Year + now.CalendarCentury ); i++ )
    {
        if( ( i == 0 ) || ( i % 4 ) == 0 )
        {
            timeCounterTemp += ( double )SecondsInLeapYear;
        }
        else
        {
            timeCounterTemp += ( double )SecondsInYear;
        }
    }

    // Months (calculation valid up to year 2099)*/
    if( ( now.CalendarDate.Year == 0 ) || ( ( now.CalendarDate.Year + now.CalendarCentury ) % 4 ) == 0 )
    {
        for( uint8_t i = 0; i < ( now.CalendarDate.Month - 1 ); i++ )
        {
            timeCounterTemp += ( double )( DaysInMonthLeapYear[i] * SecondsInDay );
        }
    }
    else
    {
        for( uint8_t i = 0;  i < ( now.CalendarDate.Month - 1 ); i++ )
        {
            timeCounterTemp += ( double )( DaysInMonth[i] * SecondsInDay );
        }
    }

    timeCounterTemp += ( double )( ( uint32_t )now.CalendarTime.Seconds +
                     ( ( uint32_t )now.CalendarTime.Minutes * SecondsInMinute ) +
                     ( ( uint32_t )now.CalendarTime.Hours * SecondsInHour ) +
                     ( ( uint32_t )( now.CalendarDate.Date * SecondsInDay ) ) );

    timeCounterTemp = ( double )timeCounterTemp * RTC_ALARM_TICK_DURATION;

    timeCounter = round( timeCounterTemp );
    return ( timeCounter );
}

static void RtcCheckCalendarRollOver( uint8_t year )
{
    if( year == 99 )
    {
        CalendarRollOverReady = true;
    }

    if( ( CalendarRollOverReady == true ) && ( ( year + Century ) == Century ) )
    {   // Indicate a roll-over of the calendar
        CalendarRollOverReady = false;
        Century = Century + 100;
    }
}

static RtcCalendar_t RtcGetCalendar( void )
{
    RtcCalendar_t calendar;
    HAL_RTC_GetTime( &RtcHandle, &calendar.CalendarTime, RTC_FORMAT_BIN );
    HAL_RTC_GetDate( &RtcHandle, &calendar.CalendarDate, RTC_FORMAT_BIN );
    calendar.CalendarCentury = Century;
    RtcCheckCalendarRollOver( calendar.CalendarDate.Year );
    return calendar;
}


/*!
 * \brief RTC IRQ Handler of the RTC Alarm
 */
void RTC_Alarm_IRQ( void )
{
    HAL_RTC_AlarmIRQHandler( &RtcHandle );
    HAL_RTC_DeactivateAlarm( &RtcHandle, RTC_ALARM_A );
    RtcRecoverMcuStatus( );
    RtcComputeWakeUpTime( );
    BlockLowPowerDuringTask( false );
    /* TimerIrqHandler( ); */
}


static void McuEnterStopMode(void)
{
    DEBUG("mcu into stop");
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
    //无用IO需设为模拟输入
    GPIO_InitStructure.Pin = GPIO_PIN_All;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

    //处理sx1278 spi 接口 IO 设为输入上拉 降低1278功耗
    GPIO_InitStructure.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStructure.Mode = INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    HAL_Pin_Mode(SX1278_NSS,INPUT_PULLUP);

#ifdef configHAL_USB_CDC_ENABLE
    USB_USART_Initial(0);
#endif
    /* __HAL_RCC_USB_FORCE_RESET();//USB 如果打开了 必须运行来降低功耗 USB_USART_Initial(0)会运行此功能*/
    __HAL_RCC_LSI_DISABLE();

    //禁用比较器
    __COMP_CLK_DISABLE();

    // 允许/禁用 调试端口 少800uA
    HAL_DBGMCU_DisableDBGStopMode();
}


void BoardDeInitMcu(void)
{
    McuEnterStopMode();
}

void BoardInitMcu(void)
{
    Set_System();
#ifdef configHAL_USB_CDC_ENABLE
    USB_USART_Initial(115200);
#endif
    HAL_UI_Initial(); //初始化三色灯
}

static void SlaveMcuEnterStopMcu(void)
{
    DEBUG("mcu into stop");
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
    //无用IO需设为模拟输入
    GPIO_InitStructure.Pin = GPIO_PIN_All;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
    HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);

    //从模式时A0外部中断唤醒
    HAL_Interrupts_Attach(A0,NULL,NULL,FALLING,NULL);

    //处理sx1278 spi 接口 IO 设为输入上拉 降低1278功耗
    GPIO_InitStructure.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStructure.Mode = INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    HAL_Pin_Mode(SX1278_NSS,INPUT_PULLUP);

#ifdef configHAL_USB_CDC_ENABLE
    USB_USART_Initial(0);
#endif
    /* __HAL_RCC_USB_FORCE_RESET();//USB 如果打开了 必须运行来降低功耗 USB_USART_Initial(0)会运行此功能*/
    __HAL_RCC_LSI_DISABLE();

    //禁用比较器
    __COMP_CLK_DISABLE();

    // 允许/禁用 调试端口 少800uA
    HAL_DBGMCU_DisableDBGStopMode();
}

void SlaveModeRtcEnterLowPowerStopMode( void )
{
    #if 0
    SlaveMcuEnterStopMcu();
    // Disable the Power Voltage Detector
    HAL_PWR_DisablePVD( );
    SET_BIT( PWR->CR, PWR_CR_CWUF );
    // Enable Ultra low power mode
    HAL_PWREx_EnableUltraLowPower( );
    // Enable the fast wake up from Ultra low power mode
    HAL_PWREx_EnableFastWakeUp( );
    // Enter Stop Mode
    HAL_PWR_EnterSTOPMode( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );
    BoardInitMcu( );
    #endif
    return;
}
