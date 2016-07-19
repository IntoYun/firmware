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

  nfo->versionString, stringify(SYSTEM_VERSION_STRING), sizeof(info->versionString));
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
*/

#include "intorobot_def.h"
#include "system_version.h"

int system_version(char *pversion)
{
#if 0
    if(NULL != pversion)
    {
        strcpy(pversion, stringify(SYSTEM_VERSION_STRING));
        return strlen(pversion)
    }
    return SDK_ERR;
#endif
    return SDK_ERR;
}
