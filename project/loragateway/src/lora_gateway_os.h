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

#ifndef LORA_GATEWAY_OS_H_
#define LORA_GATEWAY_OS_H_

#ifdef __cplusplus
extern "C"
{
#endif


typedef int32_t  ostime_t;

struct osjob_t;
typedef void (*osjobcb_t) (struct osjob_t*);

struct osjob_t {
    struct osjob_t* next;
    uint8_t *data;
    uint16_t datalen;
    void *reserved;
    ostime_t deadline;
    osjobcb_t  func;
};

void os_init (void);
ostime_t os_getTime ();
void os_clearCallback (osjob_t* job);
void os_setCallback (osjob_t* job, osjobcb_t cb);
void os_setTimedCallback (osjob_t* job, ostime_t time, osjobcb_t cb);
void os_runloop (void);

#ifdef __cplusplus
}
#endif


#endif // LORA_GATEWAY_OS_H_
