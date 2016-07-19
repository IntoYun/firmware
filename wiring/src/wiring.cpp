/**
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


#include "wiring.h"
#include "wiring_ticks.h"
#include "adc_hal.h"
#include "system_task.h"


/*
 * @brief  Override the default ADC Sample time depending on requirement
 * @param  ADC_SampleTime: The sample time value to be set.
 *
 * On Core, this parameter can be one of the following values:
 * @arg ADC_SampleTime_1Cycles5: Sample time equal to 1.5 cycles
 * @arg ADC_SampleTime_7Cycles5: Sample time equal to 7.5 cycles
 * @arg ADC_SampleTime_13Cycles5: Sample time equal to 13.5 cycles
 * @arg ADC_SampleTime_28Cycles5: Sample time equal to 28.5 cycles
 * @arg ADC_SampleTime_41Cycles5: Sample time equal to 41.5 cycles
 * @arg ADC_SampleTime_55Cycles5: Sample time equal to 55.5 cycles
 * @arg ADC_SampleTime_71Cycles5: Sample time equal to 71.5 cycles
 * @arg ADC_SampleTime_239Cycles5: Sample time equal to 239.5 cycles
 *
 * On Photon, this parameter can be one of the following values:
 * @arg ADC_SampleTime_3Cycles: Sample time equal to 3 cycles
 * @arg ADC_SampleTime_15Cycles: Sample time equal to 15 cycles
 * @arg ADC_SampleTime_28Cycles: Sample time equal to 28 cycles
 * @arg ADC_SampleTime_56Cycles: Sample time equal to 56 cycles
 * @arg ADC_SampleTime_84Cycles: Sample time equal to 84 cycles
 * @arg ADC_SampleTime_112Cycles: Sample time equal to 112 cycles
 * @arg ADC_SampleTime_144Cycles: Sample time equal to 144 cycles
 * @arg ADC_SampleTime_480Cycles: Sample time equal to 480 cycles
 *
 * @retval None
 */
void setADCSampleTime(uint8_t ADC_SampleTime)
{
  HAL_ADC_Set_Sample_Time(ADC_SampleTime);
}

long map(long value, long fromStart, long fromEnd, long toStart, long toEnd)
{
  return (value - fromStart) * (toEnd - toStart) / (fromEnd - fromStart) + toStart;
}

void delay(unsigned long ms)
{
    system_delay_ms(ms, false);
}
