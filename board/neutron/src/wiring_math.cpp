/**
 ******************************************************************************
 * @file     : wiring_math.cpp
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
#include "wiring_math.h"
extern "C" {
#include <stdlib.h>
}
/*********************************************************************************
 *Function		: int random(int max)
 *Description	: Read random number
 *Input		: none
 *Output		: none
 *Return		: none
 *author		: lz
 *date			: 2015-2-1
 *Others		: none
 **********************************************************************************/
int random(int max)
{
    if (0 == max)
    {
        return 0;
    }
    return rand() % max;
}

/*********************************************************************************
 *Function		: int random(int min, int max)
 *Description	: Minimum maximum random Numbers
 *Input		       : none
 *Output		: none
 *Return		: none
 *author		: lz
 *date			: 2015-2-1
 *Others		: none
 **********************************************************************************/
int random(int min, int max)
{
    if (min >= max)
    {
        return min;
    }
    return random(max - min) + min;
}

/*********************************************************************************
 *Function		: void randomSeed(unsigned int seed)
 *Description	: Set the random number seed
 *Input		: The random number
 *Output		: none
 *Return		: none
 *author		: lz
 *date			: 2015-2-1
 *Others		: lz
 **********************************************************************************/
void randomSeed(unsigned int seed)
{
    srand(seed);
}

/*********************************************************************************
 *Function		: long map(long value, long fromStart, long fromEnd, long toStart, long toEnd)
 *Description	: To get a random number
 *Input		      : none
 *Output		: none
 *Return		: none
 *author		: lz
 *date			: 2015-2-1
 *Others		: none
 **********************************************************************************/
long map(long value, long fromStart, long fromEnd, long toStart, long toEnd)
{
    return (value - fromStart) * (toEnd - toStart) / (fromEnd - fromStart) + toStart;
}

unsigned int makeWord(unsigned int w) { return w; }
unsigned int makeWord(unsigned char h, unsigned char l) { return (h << 8) | l; }

