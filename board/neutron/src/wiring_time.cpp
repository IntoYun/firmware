/**
 ******************************************************************************
 * @file     : wiring_time.cpp
 * @author   : robin
 * @version	 : V1.0.0
 * @date     : 6-December-2014 
 * @brief    :Time utility functions to set and get Date/Time using RTC
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

#include "wiring_time_hal.h"

#include "wiring_time.h"

#include <time.h>




static struct skew
{
    int8_t error;
    uint8_t ticks;
} skew;

/* The calendar "tm" structure from the standard libray "time.h" has the following definition: */
//struct tm
//{
//	int tm_sec;         /* seconds,  range 0 to 59          */
//	int tm_min;         /* minutes, range 0 to 59           */
//	int tm_hour;        /* hours, range 0 to 23             */
//	int tm_mday;        /* day of the month, range 1 to 31  */
//	int tm_mon;         /* month, range 0 to 11             */
//	int tm_year;        /* The number of years since 1900   */
//	int tm_wday;        /* day of the week, range 0 to 6    */
//	int tm_yday;        /* day in the year, range 0 to 365  */
//	int tm_isdst;       /* daylight saving time             */
//};

struct tm calendar_time_cache;	// a cache of calendar time structure elements
time_t unix_time_cache;  		// a cache of unix_time that was updated
time_t time_zone_cache;			// a cache of the time zone that was set   	相对格林威治时间偏移量

/* Time utility functions */
static struct tm Convert_UnixTime_To_CalendarTime(time_t unix_time);
//static time_t Convert_CalendarTime_To_UnixTime(struct tm calendar_time);
static time_t Get_UnixTime(void);
//static struct tm Get_CalendarTime(void);
static void Set_UnixTime(time_t unix_time);
//static void Set_CalendarTime(struct tm t);
static void Refresh_UnixTime_Cache(time_t unix_time);

/*******************************************************************************
 *Function       : Wiring_RTC_Interrupt_Handler (Declared as weak in stm32_it.cpp)
 *Description   : This function handles RTC global interrupt request.
 *Input            : None.
 *Output          : None.
 *Return          : None.
 *author          : lz
 *date             : 6-December-2013
 *Others:  
 *******************************************************************************/
void Wiring_RTC_Interrupt_Handler(void)
{
    // Only intervene if we have an error to correct
    if (0 != skew.error && 0 < skew.ticks)
    {
        time_t now = Get_UnixTime();
        // By default, we set the clock 1 second forward
        time_t skew_step = 1;

        if (skew.error > 0)
        {
            // Error is positive, so we need to slow down
            if (skew.ticks / skew.error < 2)
            {
                // Don't let time go backwards!
                // Hold the clock still for a second
                skew_step--;
                skew.error--;
            }
        }
        else
        {
            // Error is negative, so we need to speed up
            if (skew.ticks / skew.error > -2)
            {
                // Skip a second forward
                skew_step++;
                skew.error++;
            }
        }
        skew.ticks--;
        Set_UnixTime(now + skew_step);
    }
}

/*********************************************************************************
  *Function      : static struct tm Convert_UnixTime_To_CalendarTime(time_t unix_time)
  *Description  : Convert Unix/RTC time to Calendar time
  *Input          : unix_time
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        :         
**********************************************************************************/
static struct tm Convert_UnixTime_To_CalendarTime(time_t unix_time)
{
    struct tm *calendar_time;
    unix_time += time_zone_cache;
    calendar_time = gmtime(&unix_time); //localtime(&unix_time);
    //calendar_time->tm_year += 1900;
    calendar_time->tm_year += 1904;
    //MO_INFO(("\r\nyear : %d\r\n", calendar_time->tm_year));
    //MO_INFO(("\r\nday  : %d\r\n", calendar_time->tm_day));

    return *calendar_time;
}

/*********************************************************************************
  *Function      : static time_t Convert_CalendarTime_To_UnixTime(struct tm calendar_time)
  *Description  : Convert Calendar time to Unix/RTC time 
  *Input          : unix_time
  *Output         : none
  *Return         : none
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
/*static time_t Convert_CalendarTime_To_UnixTime(struct tm calendar_time)
{
    calendar_time.tm_year -= 1900;
    time_t unix_time = mktime(&calendar_time);
    return unix_time;
}*/

/*********************************************************************************
  *Function      : static time_t Get_UnixTime(void)
  *Description  : Get Unix/RTC time
  *Input          : none
  *Output        : none
  *Return         : none
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
static time_t Get_UnixTime(void)
{

    return  mo_Get_UnixTime_hal();

/*
    time_t unix_time = (time_t)RTC_GetCounter();
    return unix_time;*/
}

/*********************************************************************************
  *Function      : static struct tm Get_CalendarTime(void)
  *Description  : Get converted Calendar time
  *Input          : none
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        :         
**********************************************************************************/
/*static struct tm Get_CalendarTime(void)
{
    time_t unix_time = Get_UnixTime();
    unix_time += time_zone_cache;
    struct tm calendar_time = Convert_UnixTime_To_CalendarTime(unix_time);
    return calendar_time;
}*/

/*********************************************************************************
  *Function      : static void Set_UnixTime(time_t unix_time)
  *Description  : Set Unix/RTC time
  *Input          : unix_time
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        :         
**********************************************************************************/
static void Set_UnixTime(time_t unix_time)
{

	mo_Set_UnixTime_hal( unix_time);
/*
    RTC_WaitForLastTask();
    RTC_SetCounter((uint32_t)unix_time);
    RTC_WaitForLastTask();*/
}

/*********************************************************************************
  *Function      : static void Set_UnixTime(time_t unix_time)
  *Description  : Set Calendar time as Unix/RTC time 
  *Input          : unix_time
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        :         
**********************************************************************************/
/*static void Set_CalendarTime(struct tm calendar_time)
{
    Set_UnixTime(Convert_CalendarTime_To_UnixTime(calendar_time));
}*/

/*********************************************************************************
  *Function      : static void Refresh_UnixTime_Cache(time_t unix_time)
  *Description  : Refresh Unix/RTC time cache
  *Input          : unix_time
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        :         
**********************************************************************************/
static void Refresh_UnixTime_Cache(time_t unix_time)
{
    if(unix_time != unix_time_cache)
    {
        calendar_time_cache = Convert_UnixTime_To_CalendarTime(unix_time);
        unix_time_cache = unix_time;
    }
}

/*********************************************************************************
  *Function      : int TimeClass::hour(void)
  *Description  : current hour
  *Input          : none
  *Output        : none
  *Return        : the hour
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::hour(void)
{
    return hour(now());
}

/*********************************************************************************
  *Function      : int TimeClass::hour(time_t t)
  *Description  : the hour for the given time
  *Input          : none
  *Output        : none
  *Return        : the hour
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::hour(time_t t)
{
    Refresh_UnixTime_Cache(t);
    return calendar_time_cache.tm_hour;
}

/*********************************************************************************
  *Function      : int TimeClass::hourFormat12(void)
  *Description  : current hour in 12 hour format 
  *Input          : none
  *Output        : none
  *Return        : the hour
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::hourFormat12(void)
{
    return hourFormat12(now());
}

/*********************************************************************************
  *Function      : int TimeClass::hourFormat12(time_t t)
  *Description  : the hour for the given time in 12 hour format
  *Input          : none
  *Output        : none
  *Return        : the hour
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::hourFormat12(time_t t)
{
    Refresh_UnixTime_Cache(t);
    if(calendar_time_cache.tm_hour == 0)
    {return 12;}	//midnight
    else if( calendar_time_cache.tm_hour > 12)
    {return calendar_time_cache.tm_hour - 12;}
    else
    {return calendar_time_cache.tm_hour;}
}

/*********************************************************************************
  *Function      : uint8_t TimeClass::isAM(void)
  *Description  : returns true if time now is AM 
  *Input          : none
  *Output        : none
  *Return        : true: AM   false: PM
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
uint8_t TimeClass::isAM(void)
{
    return !isPM(now());
}

/* returns true the given time is AM */
/*********************************************************************************
  *Function      : uint8_t TimeClass::isAM(time_t t)
  *Description  : returns true the given time is AM 
  *Input          : none
  *Output        : none
  *Return        : true: AM   false: PM
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
uint8_t TimeClass::isAM(time_t t)
{
    return !isPM(t);
}

/*********************************************************************************
  *Function      : uint8_t TimeClass::isPM(void)
  *Description  : returns true if time now is PM
  *Input          : none
  *Output        : none
  *Return        : true: PM   false: AM
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
uint8_t TimeClass::isPM(void)
{
    return isPM(now());
}

/*********************************************************************************
  *Function      : uint8_t TimeClass::isPM(time_t t)
  *Description  : returns true the given time is PM 
  *Input          : none
  *Output        : none
  *Return        : true: PM   false: AM
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
uint8_t TimeClass::isPM(time_t t)
{
    return (hour(t) >= 12);
}

/*********************************************************************************
  *Function      : int TimeClass::minute(void)
  *Description  : current minute 
  *Input          : none
  *Output        : none
  *Return        : current minute 
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::minute(void)
{
    return minute(now());
}

/*********************************************************************************
  *Function      : int TimeClass::minute(time_t t)
  *Description  : the minute for the given time 
  *Input          : none
  *Output        : none
  *Return        : the minute for the given time 
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::minute(time_t t)
{
    Refresh_UnixTime_Cache(t);
    return calendar_time_cache.tm_min;
}

/*********************************************************************************
  *Function      : int TimeClass::second(void)
  *Description  : current seconds
  *Input          : none
  *Output        : none
  *Return        : the time
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::second(void)
{
    return second(now());
}

/*********************************************************************************
  *Function      : int TimeClass::second(time_t t)
  *Description  : the second for the given time
  *Input          : none
  *Output        : none
  *Return         : the second
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::second(time_t t)
{
    Refresh_UnixTime_Cache(t);
    return calendar_time_cache.tm_sec;
}

/*********************************************************************************
  *Function      : int TimeClass::day(void)
  *Description  : current day
  *Input          : none
  *Output        : none
  *Return        : the time
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::day(void)
{
    return day(now());
}

/*********************************************************************************
  *Function      : int TimeClass::day(time_t t)
  *Description  : the day for the given time 
  *Input          : none
  *Output        : none
  *Return        : the time
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::day(time_t t)
{
	Refresh_UnixTime_Cache(t);
	return calendar_time_cache.tm_mday;
}

/*********************************************************************************
  *Function      : int TimeClass::weekday(void)
  *Description  : the current weekday
  *Input          : none
  *Output        : none
  *Return        : the time
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::weekday(void)
{
    return weekday(now());
}

/*********************************************************************************
  *Function      : int TimeClass::weekday(time_t t)
  *Description  : the weekday for the given time
  *Input          : none
  *Output        : none
  *Return        : weekday
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::weekday(time_t t)
{
    Refresh_UnixTime_Cache(t);
    return (calendar_time_cache.tm_wday + 1);//Arduino's weekday representation
}

/*********************************************************************************
  *Function      : int TimeClass::month(void)
  *Description  : current month 
  *Input          : none
  *Output        : none
  *Return        : the month
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::month(void)
{
    return month(now());
}

/*********************************************************************************
  *Function      : int TimeClass::month(time_t t)
  *Description  : the month for the given time 
  *Input          : none
  *Output        : none
  *Return        : the month
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::month(time_t t)
{
    Refresh_UnixTime_Cache(t);
    return (calendar_time_cache.tm_mon + 1);//Arduino's month representation
}

/*********************************************************************************
  *Function      : int TimeClass::year(void)
  *Description  : current four digit year
  *Input          : none
  *Output        : none
  *Return        : digit year
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::year(void)
{
    return year(now());
}

/*********************************************************************************
  *Function      : int TimeClass::year(time_t t)
  *Description  : the year for the given time
  *Input          : none
  *Output        : none
  *Return        : digit year
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
int TimeClass::year(time_t t)
{
    Refresh_UnixTime_Cache(t);
    return calendar_time_cache.tm_year;
}

/*********************************************************************************
  *Function      : time_t TimeClass::now(void)
  *Description  : return the current time as seconds since Jan 1 1970
  *Input          : none
  *Output        : none
  *Return        : the time
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
time_t TimeClass::now(void)
{
    return Get_UnixTime();
}

/*********************************************************************************
  *Function      : void TimeClass::zone(float GMT_Offset)
  *Description  : set the time zone (+/-) offset from GMT
  *Input          : GMT_Offset(float)
  *Output        : none
  *Return        : none
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
void TimeClass::zone(float GMT_Offset)
{
    if(GMT_Offset < -12 || GMT_Offset > 13)
    {
        return;
    }
    time_zone_cache = GMT_Offset * 3600;
}

/*********************************************************************************
  *Function      : void TimeClass::setTime(time_t t)
  *Description  : set the given time as unix/rtc time
  *Input          : time_t t: the time
  *Output        : none
  *Return        : none
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
void TimeClass::setTime(time_t t)
{
	Set_UnixTime(t);

/*
    int32_t delta_error = Get_UnixTime() - t;
    if (delta_error > 127 || delta_error < -127)
    {
        // big delta, jump abruptly to the new time
        Set_UnixTime(t);
    }
    else
    {
        // small delta, gradually skew toward the new time
        skew.error = delta_error;
        skew.ticks = 2 * abs(delta_error);
    }*/
}

/*********************************************************************************
  *Function      : String TimeClass::timeStr()
  *Description  : return string representation of the current time
  *Input          : none
  *Output        : none
  *Return        : none
  *author         : lz
  *date            : 6-December-2013
  *Others         :         
**********************************************************************************/
String TimeClass::timeStr(void)
{
    return timeStr(now());
}

/*********************************************************************************
  *Function     : String TimeClass::timeStr(time_t t)
  *Description  : return string representation for the given time
  *Input          : none
  *Output        : none
  *Return        : none
  *author        : lz
  *date           : 6-December-2013
  *Others        :         
**********************************************************************************/
String TimeClass::timeStr(time_t t)
{
    struct tm *calendar_time;
    t += time_zone_cache;
    calendar_time = gmtime(&t);//localtime(&t);
    String calendar_time_string = String(asctime(calendar_time));
    return calendar_time_string;
}

TimeClass Time;

