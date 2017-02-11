/**
 ******************************************************************************
 * @file       :  openwrt_pin.h
 * @author  :  robot
 * @version :  V1.0.0
 * @date     :  2015-02-01
 * @brief     :    
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

#ifndef   OPENWRT_PIN_H_
#define   OPENWRT_PIN_H_

#include "lib_process.h"


class OpenwrtPinClass: public Process 
{
    public:
        OpenwrtPinClass();
        ~OpenwrtPinClass();
        void openwrt_pininit(void);
        void openwrt_pinmode(uint16_t pin, PinMode setMode);
        void openwrt_digital_write(uint16_t pin, uint8_t value);
        int32_t openwrt_digital_read(uint16_t pin);
};

extern OpenwrtPinClass OpenwrtPin;

#endif /*OPENWRT_PIN_H_*/

