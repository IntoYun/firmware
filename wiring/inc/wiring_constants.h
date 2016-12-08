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

#ifndef WIRING_CONSTANTS_H_
#define WIRING_CONSTANTS_H_


#include <stdint.h>
#include <type_traits>

enum PinState {
    LOW = 0,
    HIGH = 1
};

#define ON      0x1
#define OFF     0x0

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352


template <typename T, typename U>
static inline
typename std::common_type<T, U>::type
max (T a, U b) { return ((a)>(b)?(a):(b)); }

template <typename T, typename U>
static inline
typename std::common_type<T, U>::type
min (T a, U b) { return static_cast<typename std::common_type<T, U>::type>((a)<(b)?(a):(b)); }

template <typename T, typename U, typename V>
static inline
T constrain (T amt, U low, V high) { return ((amt)<(low)?(low):((amt)>(high)?(high):(amt))); }

template <typename T>
static inline
T round (T x) { return ((x)>=0?(long)((x)+0.5):(long)((x)-0.5)); }

typedef bool boolean;
typedef uint8_t byte;

#ifndef FALSE
#define FALSE					(0x00)
#endif
#ifndef TRUE
#define TRUE					(!FALSE)
#endif


#endif
