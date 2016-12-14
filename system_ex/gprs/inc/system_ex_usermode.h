/**
 ******************************************************************************
 * @file        : system_ex_usermode.h
 * @author      : robin
 * @version     : V1.0.0
 * @date        : 6-December-2014
 * @brief       :
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

#ifndef SYSTEM_EX_USERMODE_H_
#define SYSTEM_EX_USERMODE_H_


typedef enum
{
    BUTTON_MODE_NONE = 0,
    BUTTON_MODE_CONFIG,
    BUTTON_MODE_DEFFW,
    BUTTON_MODE_COM,
    BUTTON_MODE_FAC,
    BUTTON_MODE_NC,
    BUTTON_MODE_RESET
} button_mode_t;


void manage_setup_config();

#endif

