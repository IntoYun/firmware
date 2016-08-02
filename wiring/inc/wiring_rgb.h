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

#ifndef WIRING_RGB_H_
#define WIRING_RGB_H_


#include <stdint.h>
#include <functional>
#include "rgbled_hal.h"

typedef void (raw_rgb_change_handler_t)(uint8_t, uint8_t, uint8_t);
typedef std::function<raw_rgb_change_handler_t> wiring_rgb_change_handler_t;

class RGBClass {
public:
	static bool controlled(void);
	static void control(bool);
	static void color(int, int, int);
	static void color(uint32_t rgb);
	static void brightness(uint8_t, bool update=true);

	static uint8_t brightness() {
		//return Get_LED_Brightness();
        return 0;
	}

	static void onChange(wiring_rgb_change_handler_t handler);
	static void onChange(raw_rgb_change_handler_t *handler);

    template <typename T>
    static void onChange(void (T::*handler)(uint8_t, uint8_t, uint8_t), T *instance) {
      using namespace std::placeholders;
      onChange(std::bind(handler, instance, _1, _2, _3));
    }

private:
	static void call_raw_change_handler(void* data, uint8_t r, uint8_t g, uint8_t b, void* reserved);
	static void call_std_change_handler(void* data, uint8_t r, uint8_t g, uint8_t b, void* reserved);
};

extern RGBClass RGB;

#endif
