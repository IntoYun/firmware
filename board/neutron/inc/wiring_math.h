/**
 ******************************************************************************
 * @file     : wiring_math.h
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
#ifndef   WIRING_MATH_H_
#define	  WIRING_MATH_H_

#include "variant.h"

void randomSeed(unsigned int seed);
int random(int min);
int random(int min, int max);
long map(long value, long fromStart, long fromEnd, long toStart, long toEnd);

unsigned int makeWord(unsigned int w);
unsigned int makeWord(unsigned char h, unsigned char l);

#define word(...) makeWord(__VA_ARGS__)

#endif	/* WIRING_MATH_H_ */

