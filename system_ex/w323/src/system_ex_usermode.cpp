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

/* Includes ------------------------------------------------------------------*/
#include "service_debug.h"
#include "params_hal.h"
#include "system_ex_setup.h"
#include "system_ex_usermode.h"

extern "C" void HAL_SysTick_Handler(void)
{
}


// These are internal methods
void manage_setup_config(void)
{
    if(HAL_PARAMS_Get_System_config_flag())
    {
        DEBUG_D(("enter device config\r\n"));
        DeviceSetupUsb.init();
        DeviceSetupUsart.init();
        DeviceSetupUdp.init();
        while(1)
        {
            if( DeviceSetupUsb.process() )
            {
                break;
            }

            if( DeviceSetupUsart.process() )
            {
                break;
            }

            if( DeviceSetupUdp.process() )
            {
                break;
            }
            HAL_Core_System_Yield();
        }
        DEBUG_D(("exit  device config\r\n"));
        HAL_PARAMS_Set_System_config_flag(0);
        HAL_PARAMS_Save_Params();
    }
}

