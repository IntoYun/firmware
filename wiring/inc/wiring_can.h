/**
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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

#ifndef WIRING_CAN_H
#define WIRING_CAN_H_

#include "firmware_config.h"

#ifdef configWIRING_CAN_ENABLE

#include "wiring.h"
#include "can_hal.h"

class CANChannel
{
private:
  HAL_CAN_Channel _channel;

public:
  CANChannel(HAL_CAN_Channel channel,
             uint16_t rxQueueSize = 32,
             uint16_t txQueueSize = 32);
  ~CANChannel() {};
  void begin(unsigned long baudRate,
             uint32_t flags = 0);
  void end();

  uint8_t available();
  bool receive(CANMessage &message);
  bool transmit(const CANMessage &message);

  bool addFilter(uint32_t id, uint32_t mask, HAL_CAN_Filters type = CAN_FILTER_STANDARD);
  void clearFilters();

  bool isEnabled();

  HAL_CAN_Errors errorStatus();
};

#endif

#endif
