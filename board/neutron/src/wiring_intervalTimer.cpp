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
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

//Adapted for IntoRobot by Robin, Sept 19, 2015

#include "wiring_intervalTimer.h"

TIM_HandleTypeDef Timer2Handle;
TIM_HandleTypeDef Timer3Handle;
TIM_HandleTypeDef Timer4Handle;
TIM_HandleTypeDef Timer5Handle;
TIM_HandleTypeDef Timer9Handle;
TIM_HandleTypeDef Timer10Handle;
TIM_HandleTypeDef Timer11Handle;


void (*Wiring_TIM2_Interrupt_Handler)(void);
void (*Wiring_TIM3_Interrupt_Handler)(void);
void (*Wiring_TIM4_Interrupt_Handler)(void);
void (*Wiring_TIM5_Interrupt_Handler)(void);
void (*Wiring_TIM9_Interrupt_Handler)(void);
void (*Wiring_TIM10_Interrupt_Handler)(void);
void (*Wiring_TIM11_Interrupt_Handler)(void);


// ------------------------------------------------------------
// static class variables need to be reiterated here before use
// ------------------------------------------------------------
bool IntoRobotIntervalTimer::SIT_used[];
IntoRobotIntervalTimer::ISRcallback IntoRobotIntervalTimer::SIT_CALLBACK[];

// ------------------------------------------------------------
// Define interval timer ISR hooks for three available timers
// TIM2, TIM3 and TIM4 with callbacks to user code.
// These default as infinite loop stubs by Spark
// ------------------------------------------------------------

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance == TIM2)
	{
		//Wiring_TIM2_Interrupt_Handler();
		IntoRobotIntervalTimer::SIT_CALLBACK[0]();
	}
	else if(htim->Instance == TIM3)
	{
		//Wiring_TIM3_Interrupt_Handler();
		IntoRobotIntervalTimer::SIT_CALLBACK[1]();
	}
	else if(htim->Instance == TIM4)
	{
		//Wiring_TIM4_Interrupt_Handler();
		IntoRobotIntervalTimer::SIT_CALLBACK[2]();
	}
	else if(htim->Instance == TIM5)
	{
		//Wiring_TIM5_Interrupt_Handler();
		IntoRobotIntervalTimer::SIT_CALLBACK[3]();
	}
	else if(htim->Instance == TIM9)
	{
		//Wiring_TIM9_Interrupt_Handler();
		IntoRobotIntervalTimer::SIT_CALLBACK[4]();
	}
	else if(htim->Instance == TIM10)
	{
		//Wiring_TIM10_Interrupt_Handler();
		IntoRobotIntervalTimer::SIT_CALLBACK[5]();
	}
	else if(htim->Instance == TIM11)
	{
		//Wiring_TIM11_Interrupt_Handler();
		IntoRobotIntervalTimer::SIT_CALLBACK[6]();
	}
}


extern "C" void Wiring_TIM2_Interrupt_Handler_override()
{
	//ISR for TIM2 code here
	IntoRobotIntervalTimer::SIT_CALLBACK[0]();
}

extern "C" void Wiring_TIM3_Interrupt_Handler_override()
{
	//ISR code for TIM3 here
	IntoRobotIntervalTimer::SIT_CALLBACK[1]();
}

extern "C" void Wiring_TIM4_Interrupt_Handler_override()
{
	//ISR code for TIM3 here
	IntoRobotIntervalTimer::SIT_CALLBACK[2]();
}

extern "C" void Wiring_TIM5_Interrupt_Handler_override()
{
	//ISR code for TIM3 here
	IntoRobotIntervalTimer::SIT_CALLBACK[3]();
}

extern "C" void Wiring_TIM9_Interrupt_Handler_override()
{
	//ISR code for TIM3 here
	IntoRobotIntervalTimer::SIT_CALLBACK[4]();
}

extern "C" void Wiring_TIM10_Interrupt_Handler_override()
{
	//ISR code for TIM3 here
	IntoRobotIntervalTimer::SIT_CALLBACK[5]();
}

extern "C" void Wiring_TIM11_Interrupt_Handler_override()
{
	//ISR code for TIM3 here
	IntoRobotIntervalTimer::SIT_CALLBACK[6]();
}


// ------------------------------------------------------------
// this function inits and starts the timer, using the specified
// function as a callback and the period provided. must be passed
// the name of a function taking no arguments and returning void.
// make sure this function can complete within the time allowed.
// attempts to allocate a timer using available resources,
// returning true on success or false in case of failure.
// Period units is defined by scale, where scale = uSec or hmSec
// and = 1-65535 microsecond (uSec)
// or 1-65535 0.5ms increments (hmSec)
// ------------------------------------------------------------
bool IntoRobotIntervalTimer::beginCycles(void (*isrCallback)(), uint16_t Period, bool scale, TIMid id) {

	// if this interval timer is already running, stop and deallocate it
	if (status == TIMER_SIT) {
		stop_SIT();
		status = TIMER_OFF;
	}
	// store callback pointer
	myISRcallback = isrCallback;

	if (id > NUM_SIT) {		// Allocate specified timer (id=0 to 3) or auto-allocate from pool (id=255)
		// attempt to allocate this timer
		if (allocate_SIT(Period, scale, AUTO)) status = TIMER_SIT;		//255 means allocate from pool
		else status = TIMER_OFF;
	}
	else {
		// attempt to allocate this timer
		if (allocate_SIT(Period, scale, id)) status = TIMER_SIT;		//0-3 so directly allocate
		else status = TIMER_OFF;
	}

	// check for success and return
	if (status != TIMER_OFF) return true;
	return false;

}


// ------------------------------------------------------------
// enables the SIT clock if not already enabled, then checks to
// see if any SITs are available for use. if one is available,
// it's initialized and started with the specified value, and
// the function returns true, otherwise it returns false
// ------------------------------------------------------------
bool IntoRobotIntervalTimer::allocate_SIT(uint16_t Period, bool scale, TIMid id) {

	if (id < NUM_SIT) {		// Allocate specified timer (id=TIMER2/3/4) or auto-allocate from pool (id=AUTO)
		if (!SIT_used[id]) {
			SIT_id = id;
			start_SIT(Period, scale);
			SIT_used[id] = true;
			return true;
		}
	}
	else {
		// Auto allocate - check for an available SIT, and if so, start it
		for (uint8_t tid = 0; tid < NUM_SIT; tid++) {
			if (!SIT_used[tid]) {
				SIT_id = tid;
				start_SIT(Period, scale);
				SIT_used[tid] = true;
				return true;
			}
		}
	}

	// Specified or no auto-allocate SIT available
	return false;
}



// ------------------------------------------------------------
// configuters a SIT's TIMER registers, etc and enables
// interrupts, effectively starting the timer upon completion
// ------------------------------------------------------------
void IntoRobotIntervalTimer::start_SIT(uint16_t timerPeriod, bool scale) {

	uint16_t prescaler;
	//TIM_TypeDef* TIMx;

	uint16_t Period;

	IRQn_Type IRQx;

	// Initialize Timer
	switch (scale)
	{
		case uSec:
			prescaler = SIT_PRESCALERu;	// Set prescaler for 1MHz clock, 1us period
			Period = timerPeriod;
			break;
		case hmSec:
			prescaler = SIT_PRESCALERm;	// Set prescaler for 2Hz clock, .5ms period
			Period = timerPeriod*2;
			if(Period > 65535) Period = 65535;
			break;
		default:
			scale == uSec;				// Default to microseconds
			prescaler = SIT_PRESCALERu;
			Period = timerPeriod;
			break;
	}

	//use SIT_id to identify TIM#
	switch (SIT_id)
	{
		case 0:		// TIM2

			__HAL_RCC_TIM2_CLK_ENABLE();
			IRQx = TIM2_IRQn;

			Timer2Handle.Instance				= TIM2;
			Timer2Handle.Init.Prescaler         = prescaler;
			Timer2Handle.Init.Period            = Period;
			Timer2Handle.Init.ClockDivision     = 0;
			Timer2Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
			Timer2Handle.Init.RepetitionCounter = 0;

			if(HAL_TIM_Base_Init(&Timer2Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			if(HAL_TIM_Base_Start_IT(&Timer2Handle) != HAL_OK)
			{
			   /* Starting Error */
			   //Error_Handler();
			}

			break;

		case 1:		// TIM3

			__HAL_RCC_TIM3_CLK_ENABLE();
			IRQx = TIM3_IRQn;

			Timer3Handle.Instance				= TIM3;
			Timer3Handle.Init.Prescaler         = prescaler;
			Timer3Handle.Init.Period            = Period;
			Timer3Handle.Init.ClockDivision     = 0;
			Timer3Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
			Timer3Handle.Init.RepetitionCounter = 0;

			if(HAL_TIM_Base_Init(&Timer3Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			if(HAL_TIM_Base_Start_IT(&Timer3Handle) != HAL_OK)
			{
			   /* Starting Error */
			   //Error_Handler();
			}

			break;

		case 2:		// TIM4

			__HAL_RCC_TIM4_CLK_ENABLE();
			IRQx = TIM4_IRQn;

			Timer4Handle.Instance				= TIM4;
			Timer4Handle.Init.Prescaler         = prescaler;
			Timer4Handle.Init.Period            = Period;
			Timer4Handle.Init.ClockDivision     = 0;
			Timer4Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
			Timer4Handle.Init.RepetitionCounter = 0;

			if(HAL_TIM_Base_Init(&Timer4Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			if(HAL_TIM_Base_Start_IT(&Timer4Handle) != HAL_OK)
			{
			   /* Starting Error */
			   //Error_Handler();
			}

			break;

		case 3:		// TIM5

			__HAL_RCC_TIM5_CLK_ENABLE();
			IRQx = TIM5_IRQn;

			Timer5Handle.Instance				= TIM5;
			Timer5Handle.Init.Prescaler         = prescaler;
			Timer5Handle.Init.Period            = Period;
			Timer5Handle.Init.ClockDivision     = 0;
			Timer5Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
			Timer5Handle.Init.RepetitionCounter = 0;

			if(HAL_TIM_Base_Init(&Timer5Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			if(HAL_TIM_Base_Start_IT(&Timer5Handle) != HAL_OK)
			{
			   /* Starting Error */
			   //Error_Handler();
			}

			break;

		case 4:		// TIM9

			__HAL_RCC_TIM9_CLK_ENABLE();
			IRQx = TIM1_BRK_TIM9_IRQn;

			Timer9Handle.Instance				= TIM9;
			Timer9Handle.Init.Prescaler         = prescaler;
			Timer9Handle.Init.Period            = Period;
			Timer9Handle.Init.ClockDivision     = 0;
			Timer9Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
			Timer9Handle.Init.RepetitionCounter = 0;

			if(HAL_TIM_Base_Init(&Timer9Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			if(HAL_TIM_Base_Start_IT(&Timer9Handle) != HAL_OK)
			{
			   /* Starting Error */
			   //Error_Handler();
			}

			break;

		case 5:		// TIM10

			__HAL_RCC_TIM10_CLK_ENABLE();
			IRQx = TIM1_UP_TIM10_IRQn;

			Timer10Handle.Instance				= TIM10;
			Timer10Handle.Init.Prescaler         = prescaler;
			Timer10Handle.Init.Period            = Period;
			Timer10Handle.Init.ClockDivision     = 0;
			Timer10Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
			Timer10Handle.Init.RepetitionCounter = 0;

			if(HAL_TIM_Base_Init(&Timer10Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			if(HAL_TIM_Base_Start_IT(&Timer10Handle) != HAL_OK)
			{
			   /* Starting Error */
			   //Error_Handler();
			}

			break;

		case 6:		// TIM11

			__HAL_RCC_TIM11_CLK_ENABLE();
			IRQx = TIM1_TRG_COM_TIM11_IRQn;

			Timer11Handle.Instance				 = TIM11;
			Timer11Handle.Init.Prescaler         = prescaler;
			Timer11Handle.Init.Period            = Period;
			Timer11Handle.Init.ClockDivision     = 0;
			Timer11Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
			Timer11Handle.Init.RepetitionCounter = 0;

			if(HAL_TIM_Base_Init(&Timer11Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			if(HAL_TIM_Base_Start_IT(&Timer11Handle) != HAL_OK)
			{
			   /* Starting Error */
			   //Error_Handler();
			}

			break;
	}

	// point to the correct SIT ISR
	SIT_CALLBACK[SIT_id] = myISRcallback;

	HAL_NVIC_SetPriority(IRQx,10, 0); // 14 or 13, which one
	HAL_NVIC_EnableIRQ(IRQx);

}


// ------------------------------------------------------------
// stop the timer if it's currently running, using its status
// to determine what hardware resources the timer may be using
// ------------------------------------------------------------
void IntoRobotIntervalTimer::end() {
	if (status == TIMER_SIT) stop_SIT();
	status = TIMER_OFF;
}


// ------------------------------------------------------------
// stops an active SIT by disabling its interrupt and TIMER
// and freeing up its state for future use.
// ------------------------------------------------------------
void IntoRobotIntervalTimer::stop_SIT() {

	IRQn_Type IRQx;

	//use SIT_id to identify TIM#
	switch (SIT_id) {
	case 0:		// TIM2
		HAL_TIM_Base_Stop(&Timer2Handle);
		IRQx = TIM2_IRQn;
		break;
	case 1:		// TIM3
		HAL_TIM_Base_Stop(&Timer3Handle);
		IRQx = TIM3_IRQn;
		break;
	case 2:		// TIM4
		HAL_TIM_Base_Stop(&Timer4Handle);
		IRQx = TIM4_IRQn;
		break;
	case 3:		// TIM5
		HAL_TIM_Base_Stop(&Timer5Handle);
		IRQx = TIM5_IRQn;
		break;
	case 4:		// TIM9
		HAL_TIM_Base_Stop(&Timer9Handle);
		IRQx = TIM1_BRK_TIM9_IRQn;
		break;
	case 5:		// TIM10
		HAL_TIM_Base_Stop(&Timer10Handle);
		IRQx = TIM1_UP_TIM10_IRQn;
		break;
	case 6:		// TIM11
		HAL_TIM_Base_Stop(&Timer11Handle);
		IRQx = TIM1_TRG_COM_TIM11_IRQn;
		break;
	}


	HAL_NVIC_DisableIRQ(IRQx);

	// free SIT for future use
	SIT_used[SIT_id] = false;
}


// ------------------------------------------------------------
// Enables or disables an active SIT's interrupt without
// removing the SIT.
// ------------------------------------------------------------
void IntoRobotIntervalTimer::interrupt_SIT(action ACT)
{

	IRQn_Type IRQx;

	//use SIT_id to identify TIM#
	switch (SIT_id)
	{
		case 0:		// TIM2
			IRQx = TIM2_IRQn;
			break;
		case 1:		// TIM3
			IRQx = TIM3_IRQn;
			break;
		case 2:		// TIM4
			IRQx = TIM4_IRQn;
			break;
		case 3:		// TIM5
			IRQx = TIM5_IRQn;
			break;
		case 4:		// TIM9
			IRQx = TIM1_BRK_TIM9_IRQn;
			break;
		case 5:		// TIM10
			IRQx = TIM1_UP_TIM10_IRQn;
			break;
		case 6:		// TIM11
			IRQx = TIM1_TRG_COM_TIM11_IRQn;
			break;

	}

	switch (ACT)
	{
		case INT_ENABLE:
			//Enable Timer Interrupt
			//HAL_NVIC_SetPriority(IRQx,10, 0); // 14 or 13, which one
			HAL_NVIC_EnableIRQ(IRQx);
			break;
		case INT_DISABLE:
			// disable interrupt
			HAL_NVIC_DisableIRQ(IRQx);
			break;
		default:
			//Do nothing
			break;
	}
}



// ------------------------------------------------------------
// Set new period for the SIT without
// removing the SIT.
// ------------------------------------------------------------
void IntoRobotIntervalTimer::resetPeriod_SIT(uint16_t newPeriod, bool scale)
{
	uint16_t prescaler;
	uint16_t Period;

	switch (scale)
	{
		case uSec:
			prescaler = SIT_PRESCALERu;	// Set prescaler for 1MHz clock, 1us period
			Period = newPeriod;
			break;
		case hmSec:
			prescaler = SIT_PRESCALERm;	// Set prescaler for 2Hz clock, .5ms period
			Period = newPeriod*2;
			if(Period > 65535) Period = 65535;
			break;
		default:
			scale == uSec;				// Default to microseconds
			prescaler = SIT_PRESCALERu;
			Period = newPeriod;
			break;
	}

	//use SIT_id to identify TIM#
	switch (SIT_id)
	{
		case 0:		// TIM2

			Timer2Handle.Instance				= TIM2;
			Timer2Handle.Init.Prescaler         = prescaler;
			Timer2Handle.Init.Period            = Period;

			if(HAL_TIM_Base_Init(&Timer2Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			__HAL_TIM_CLEAR_IT(&Timer2Handle, TIM_IT_UPDATE);

			break;

		case 1:		// TIM3

			Timer3Handle.Instance				= TIM3;
			Timer3Handle.Init.Prescaler         = prescaler;
			Timer3Handle.Init.Period            = Period;

			if(HAL_TIM_Base_Init(&Timer3Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			__HAL_TIM_CLEAR_IT(&Timer3Handle, TIM_IT_UPDATE);

			break;

		case 2:		// TIM4

			Timer4Handle.Instance				= TIM4;
			Timer4Handle.Init.Prescaler         = prescaler;
			Timer4Handle.Init.Period            = Period;

			if(HAL_TIM_Base_Init(&Timer4Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			__HAL_TIM_CLEAR_IT(&Timer4Handle, TIM_IT_UPDATE);

			break;

		case 3:		// TIM5

			Timer5Handle.Instance				= TIM5;
			Timer5Handle.Init.Prescaler         = prescaler;
			Timer5Handle.Init.Period            = Period;

			if(HAL_TIM_Base_Init(&Timer5Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			__HAL_TIM_CLEAR_IT(&Timer5Handle, TIM_IT_UPDATE);

			break;

		case 4:		// TIM9
			Timer9Handle.Instance				= TIM9;
			Timer9Handle.Init.Prescaler         = prescaler;
			Timer9Handle.Init.Period            = Period;

			if(HAL_TIM_Base_Init(&Timer9Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			__HAL_TIM_CLEAR_IT(&Timer9Handle, TIM_IT_UPDATE);

			break;

		case 5:		// TIM10

			Timer10Handle.Instance				= TIM10;
			Timer10Handle.Init.Prescaler         = prescaler;
			Timer10Handle.Init.Period            = Period;

			if(HAL_TIM_Base_Init(&Timer10Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			__HAL_TIM_CLEAR_IT(&Timer10Handle, TIM_IT_UPDATE);

			break;

		case 6:		// TIM11
			Timer11Handle.Instance				 = TIM11;
			Timer11Handle.Init.Prescaler         = prescaler;
			Timer11Handle.Init.Period            = Period;

			if(HAL_TIM_Base_Init(&Timer11Handle) != HAL_OK)
			{
				/* Initialization Error */
				//Error_Handler();
			}

			__HAL_TIM_CLEAR_IT(&Timer10Handle, TIM_IT_UPDATE);

			break;
	}


	/*TIMx->ARR = newPeriod;
	TIMx->PSC = prescaler;
	TIMx->EGR = TIM_PSCReloadMode_Immediate;
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);*/

}


// ------------------------------------------------------------
// Returns -1 if timer not allocated or sid number:
// 0 = TMR2, 1 = TMR3, 2 = TMR4
// ------------------------------------------------------------
int8_t IntoRobotIntervalTimer::isAllocated_SIT(void)
{
	if (status == TIMER_SIT)
		return SIT_id;
	else
		return -1;
}
