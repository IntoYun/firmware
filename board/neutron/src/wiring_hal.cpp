//=================================================================================================================
//input
/****************************************************************************
 *Private Included Files
 ****************************************************************************/

#include <sys/time.h>
#include <unistd.h>
#include "lib_system_all.h"

#include "cmsis_os.h"
 
 //=================================================================================================================
//come true hidden
/************************************************************************************
* Private Types
************************************************************************************/


/************************************************************************************
* Private Variables
************************************************************************************/


/************************************************************************************
* Private Functions
************************************************************************************/


//=================================================================================================================
//come true export
/************************************************************************************
* Public Type
************************************************************************************/



/************************************************************************************
*  Public Functions
************************************************************************************/

/*
功能:
	返回毫秒为单位的系统计时
//millis
*/
unsigned int mo_millis_hal(void)
{

  
	 	return HAL_GetTick();


    #if 0
    struct timeval start;
    gettimeofday(&start, NULL);

    unsigned long t_ms;
    t_ms = ( (start.tv_sec) * 1000) + ((start.tv_usec) / 1000) ;
    return t_ms;
    #endif
}

/*
功能:
	返回微秒为单位的系统计时

*/
unsigned long mo_micros_hal(void)
{

    MO_ERROR(("not support"));
    return 0;

#if 0
    struct timeval start;
    gettimeofday(&start, NULL);

    unsigned long t_us;
    t_us = ( (start.tv_sec) * 1000*1000) + (start.tv_usec) ;
    return t_us;
#endif

}

void mo_delay_hal(unsigned long ms)
{
	delay(ms);

}

/*
功能:
	返回脉冲宽度单位微秒
*/
uint32_t mo_pulseIn_hal(uint8_t pin, uint8_t state, uint32_t timeout)
{
    MO_ERROR(("not support"));
    return 0;
}


uint8_t mo_timerIsEnd_hal(uint32_t timerID, uint32_t time)
{
    volatile uint32_t current_millis = mo_millis_hal();
    volatile unsigned long elapsed_millis = current_millis - timerID;
    
    //Check for wrapping
    if (elapsed_millis < 0)
    {        
        elapsed_millis =  0xFFFFFFFF+current_millis-timerID;  
    }

    if (elapsed_millis >= (long)time)
    {
        return 1;
    }
    return 0;
}



/*
void time_test()
{
	 static uint32_t time_count;
	if(mo_timerIsEnd_hal(time_count,5000))
	{
		time_count=mo_millis_hal();
		
		MO_ERROR(("system time:%u \n",mo_millis_hal()));
	}	
}
*/







//=================================================================================================================
//output
/****************************************************************************
*Public Included Files
****************************************************************************/

//=================================================================================================================




