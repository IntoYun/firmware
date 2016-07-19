/**
 ******************************************************************************
 * @file     : wiring_constants.h
 * @author   : robin
 * @version	 : V1.0.0
 * @date     : 6-December-2014
 * @brief    :
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
#ifndef   WIRING_CONSTANTS_H_
#define	  WIRING_CONSTANTS_H_

/*
* Basic variables
*/

#if !defined(min)
#define min(a,b)                ((a)<(b)?(a):(b))
#endif
#if !defined(max)
#define max(a,b)                ((a)>(b)?(a):(b))
#endif
#if !defined(constrain)
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#endif
#if !defined(round)
#define round(x)                ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#endif

#define HIGH 0x1
#define LOW 0x0

#define boolean bool

//#define NULL ((void *)0)
#define NONE ((uint8_t)0xFF)

#endif	/* WIRING_CONSTANTS_H_ */

