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


#ifndef PANIC_H_
#define PANIC_H_
#ifdef __cplusplus
extern "C" {
#endif

#define def_panic_codes(_class,led,code) code,
typedef enum {
        NotUsedPanicCode = 0, // Not used
#include "panic_codes.h"
} ePanicCode;
#undef def_panic_codes

void panic_(ePanicCode code, void* extraInfo, void (*HAL_Delay_Microseconds)(uint32_t));

#ifdef __cplusplus
}
#endif

#endif /* PANIC_H_ */
