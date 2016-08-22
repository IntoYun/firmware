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

#ifndef SYSTEM_VERSION_H_
#define SYSTEM_VERSION_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SYSTEM_VERSION_v131  "1.3.1.160704"
#define SYSTEM_VERSION_v140  "1.4.0.160812"

#define SYSTEM_VERSION  SYSTEM_VERSION_v140


int system_version(char *pversion);

#ifdef __cplusplus
}
#endif

#endif	/* __SYSTEM_VERSION_H */

