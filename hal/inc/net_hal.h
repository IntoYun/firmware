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


#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t HAL_NET_SetNetWatchDog(uint32_t timeOutInuS);

/**
 * Notification that the wifi network has been connected to.
 */
void HAL_NET_notify_connected();
void HAL_NET_notify_disconnected();

/**
 * Notification that an IP address has been received via DHCP.
 * todo - what with the case of static IP config?
 */
void HAL_NET_notify_dhcp(bool dhcp);

void HAL_NET_notify_can_shutdown();


#ifdef __cplusplus
}
#endif
